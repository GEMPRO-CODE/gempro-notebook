import sys
import hashlib
import subprocess
import json
import os
from pathlib import Path
import re
import argparse
from concurrent.futures import ThreadPoolExecutor
import tempfile

CACHE_VERSION = 4

def cache_key(template):
    return hashlib.md5(f'{CACHE_VERSION}\0{template}'.encode()).hexdigest()

def remove_comments_from_line(line):
    return line.split('//')[0]

def remove_comments(template):
    return '\n'.join(remove_comments_from_line(line) for line in template.split('\n'))

def process(template):
    return template.split('// begin template //')[-1].split('// end template //')[0].rstrip().lstrip()

def process2(template):
    return template.replace('// begin template //', 'namespace gempro::notebook {').replace('// end template //', '};\nusing namespace gempro::notebook;').rstrip().lstrip()

def hash(template):
    processed = subprocess.run("cpp -dD -P -fpreprocessed".split(), input=template.encode(), capture_output=True)
    template = processed.stdout.decode()
    template = template.replace('\n', '').replace(' ', '').replace('\t', '')
    return hashlib.md5(template.encode()).hexdigest()[:6]

def cached_template(template, name):
    template_hash = cache_key(template)
    if os.path.exists(f"./out/template-cache/{template_hash}.tex"):
        with open(f"./out/template-cache/{template_hash}.tex", "r") as f:
            data = f.read()
            # Reuse existing caches, but don't reuse a caption from another filename.
            if data and f' - {name} - ' in data.splitlines()[0]:
                return data
    return None

def save_cached_template(template, name, data):
    template_hash = cache_key(template)
    # Identical source files can share a cache key. Publish only complete entries.
    with tempfile.NamedTemporaryFile(mode='w', dir='out/template-cache', delete=False) as f:
        temporary = Path(f.name)
        try:
            f.write(data)
            f.close()
            temporary.replace(f"./out/template-cache/{template_hash}.tex")
        finally:
            temporary.unlink(missing_ok=True)

def visit(node):
    method_list = []
    kind = node.get('kind')
    if kind == 'CXXRecordDecl' and node.get('definitionData', {}).get('isLambda'):
        return method_list
    children = node.get('inner', [])
    # Hash a named lambda's whole declaration, including its capture list.
    lambdas = [c for c in children if c.get('kind') == 'LambdaExpr']
    if kind == 'VarDecl' and lambdas:
        children = [c for lam in lambdas for c in lam.get('inner', [])
                    if c.get('kind') == 'CompoundStmt']
        is_callable = True
    elif kind == 'LambdaExpr':
        # The closure class repeats the body, including instantiated operators.
        children = [c for c in children if c.get('kind') == 'CompoundStmt']
        is_callable = True
    else:
        is_callable = (kind in ('CXXMethodDecl', 'FunctionDecl')
                       and not node.get('isImplicit')
                       and any(c.get('kind') == 'CompoundStmt' for c in children))
    if is_callable:
        span = node.get('range', {})
        begin, end = span.get('begin', {}), span.get('end', {})
        if 'offset' in begin and 'offset' in end:
            method_list.append((begin['offset'], end['offset'] + end.get('tokLen', 1)))
    for child in children:
        method_list += visit(child)
    return method_list

def dedup(methods):
    ret = []
    last_l = -1
    last_r = -1
    for l, r in methods:
        if l == last_l and r == last_r:
            continue
        ret.append((l, r))
        last_l = l
        last_r = r
    return ret

def add_method_hashes(full_template, template, name):
    offset = full_template.index(template)
    source = Path(name) if name.startswith("content/") else Path("content") / name
    result = subprocess.run(
        "clang++ -x c++ -Xclang -ast-dump=json -Xclang -ast-dump-filter=gempro::notebook -fsyntax-only -std=c++20 -stdlib=libc++ -".split() + ["-iquote", str(source.parent.resolve())],
        input=full_template.encode(),
        capture_output=True,
    )
    assert result.returncode == 0, f"Clang++ failed to process {name}. {result.stderr.decode()}"
    ast = result.stdout.decode()
    assert ast is not None, f"Clang++ produced no AST for {name}."
    data = json.loads(ast)
    data = data['inner']
    methods = []
    for node in data:
        methods += visit(node)
    methods.sort(key=lambda x: x[0])
    methods = dedup(methods)
    edits = []
    encoded = full_template.encode()
    for begin, end in methods:
        # Clang offsets count UTF-8 bytes; Python slices count characters.
        begin = len(encoded[:begin].decode()) - offset
        end = len(encoded[:end].decode()) - offset
        if not 0 <= begin < end <= len(template):
            continue
        # Clang's method range ends at the closing brace, before a lambda's ';'.
        suffix = re.match(r'(?:\s|//[^\n]*\n|/\*[\s\S]*?\*/)*;', template[end:])
        if suffix:
            end += suffix.end()
        method_body = template[begin:end]
        if '\n' not in method_body:
            continue
        method_hash = hash(method_body)
        rest = template[end:].split('\n', 1)[0].strip()
        # A line comment must not swallow another declaration or expression.
        comment = (f" /* {method_hash} */" if rest and not rest.startswith('//')
                   else f" // {method_hash}")
        edits.append((end, comment))
    # Nested bodies overlap: calculate all hashes before inserting any comments.
    for end, comment in sorted(edits, reverse=True):
        template = template[:end] + comment + template[end:]
    return template

def generate(base_template, output, name):
    template = process(base_template)
    cached = cached_template(base_template, name)
    if cached is not None:
        with open(output, "w") as f:
            f.write(cached)
        return
    full_template = process2(base_template)
    template = add_method_hashes(full_template, template, name)
    data = f"""\\captionof{{listing}}{{{hash(template)} - {name} - {len(template.splitlines())} lines}}
\\begin{{minted}}{{cpp}}
{template}
\\end{{minted}}
"""
    with open(output, "w") as f:
        f.write(data)
    save_cached_template(base_template, name, data)

def prepare_all(jobs=1):
    # Discover filenames in the notebook entries, excluding standalone minted code.
    names = set()
    for source in Path('content').rglob('*.tex'):
        text = re.sub(r'(?<!\\)%[^\n]*', '', source.read_text())
        names.update(re.findall(r'\{([^{}\s]+/[^{}\s]+\.cpp)\}', text))
    tasks = []
    for name in sorted(names):
        source = Path(name) if name.startswith('content/') else Path('content') / name
        template = source.read_text()
        if '// begin template //' not in template:
            continue
        output = Path('out/prepared') / (name + '.tex')
        output.parent.mkdir(parents=True, exist_ok=True)
        tasks.append((template, output, name))
    # Clang and cpp do the expensive work in subprocesses, outside Python's GIL.
    with ThreadPoolExecutor(max_workers=jobs) as executor:
        for _ in executor.map(lambda task: generate(*task), tasks):
            pass

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Prepare notebook listings and hashes.')
    parser.add_argument('--all', action='store_true', help='prepare all notebook snippets')
    parser.add_argument('--jobs', type=int, default=min(4, os.cpu_count() or 1),
                        help='maximum concurrent preparation jobs (default: up to 4)')
    parser.add_argument('output', nargs='?')
    parser.add_argument('name', nargs='?')
    args = parser.parse_args()
    if args.jobs < 1:
        parser.error('--jobs must be positive')
    if args.all and (args.output or args.name):
        parser.error('--all cannot be combined with output/name')
    if not args.all and (not args.output or not args.name):
        parser.error('provide --all or both output and name')
    os.makedirs("./out/template-cache/", exist_ok=True)
    if args.all:
        prepare_all(args.jobs)
    else:
        generate(sys.stdin.read(), args.output, args.name)
