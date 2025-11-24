import sys
import hashlib
from sys import argv
import subprocess
import json
import os

def remove_comments_from_line(line):
    return line.split('//')[0]

def remove_comments(template):
    return '\n'.join(remove_comments_from_line(line) for line in template.split('\n'))

def process(template):
    return template.split('// begin template //')[-1].split('// end template //')[0].rstrip().lstrip()

def process2(template):
    return template.replace('// begin template //', 'namespace gempro::notebook {').replace('// end template //', '};\nusing namespace gempro::notebook;').rstrip().lstrip()

def hash(template):
    template = remove_comments(template).replace('\n', '').replace(' ', '').replace('\t', '')
    return hashlib.md5(template.encode()).hexdigest()[:6]

def cached_template(template):
    template_hash = hashlib.md5(template.encode()).hexdigest()
    if os.path.exists(f"./out/template-cache/{template_hash}.tex"):
        with open(f"./out/template-cache/{template_hash}.tex", "r") as f:
            return f.read()
    return None

def save_cached_template(template, data):
    template_hash = hashlib.md5(template.encode()).hexdigest()
    with open(f"./out/template-cache/{template_hash}.tex", "w") as f:
        f.write(data)

def visit(node):
    method_list = []
    if 'kind' not in node:
        return method_list
    elif node['kind'] == 'CXXMethodDecl' or node['kind'] == 'FunctionDecl':
        if 'isImplicit' not in node or not node['isImplicit']:
            method_range = node.get('range', {})
            method_list.append((method_range['begin']['offset'], method_range['end']['offset'] + 1))
    else:
        for child in node.get('inner', []):
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

def add_method_hashes(full_template, template):
    offset = full_template.index(template)
    result = subprocess.run(
        "clang++ -x c++ -Xclang -ast-dump=json -Xclang -ast-dump-filter=gempro::notebook -fsyntax-only -std=c++20 -stdlib=libc++ -".split(),
        input=full_template.encode(),
        capture_output=True,
    )
    assert result.returncode == 0, f"Clang++ failed to process {argv[1]}. {result.stderr.decode()}"
    ast = result.stdout.decode()
    assert ast is not None, f"Clang++ produced no AST for {argv[1]}."
    data = json.loads(ast)
    data = data['inner']
    methods = []
    for node in data:
        methods += visit(node)
    methods.sort(key=lambda x: x[0])
    methods = dedup(methods)
    for begin, end in reversed(methods):
        begin -= offset
        end -= offset
        method_body = template[begin:end]
        if '\n' not in method_body:
            continue
        method_hash = hash(template[begin:end])
        template = template[:end] + f" // {method_hash}" + template[end:]
    return template

def generate(base_template):
    template = process(base_template)
    cached = cached_template(base_template)
    if cached is not None:
        with open("/tmp/gempro-prepared-template.tex", "w") as f:
            f.write(cached)
        return
    full_template = process2(base_template)
    template = add_method_hashes(full_template, template)
    data = f"""\\captionof{{listing}}{{{hash(template)} - {argv[1]} - {len(template.splitlines())} lines}}
\\begin{{minted}}{{cpp}}
{template}
\\end{{minted}}
"""
    with open("/tmp/gempro-prepared-template.tex", "w") as f:
        f.write(data)
    save_cached_template(base_template, data)

if __name__ == "__main__":
    template = sys.stdin.read()
    os.makedirs("./out/template-cache/", exist_ok=True)
    generate(template)
