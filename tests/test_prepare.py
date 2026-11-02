import importlib.util
import os
from pathlib import Path
import tempfile
import unittest
import threading
import shutil
import subprocess
from unittest.mock import patch


spec = importlib.util.spec_from_file_location(
    'prepare', Path(__file__).resolve().parents[1] / 'scripts/prepare.py'
)
prepare = importlib.util.module_from_spec(spec)
spec.loader.exec_module(prepare)


class PreparationTests(unittest.TestCase):
    def setUp(self):
        self.directory = tempfile.TemporaryDirectory()
        self.previous = Path.cwd()
        os.chdir(self.directory.name)
        Path('out/template-cache').mkdir(parents=True)

    def tearDown(self):
        os.chdir(self.previous)
        self.directory.cleanup()

    @unittest.skipUnless(shutil.which('clang++'), 'Clang required')
    def test_snippet_can_include_relative_dependency(self):
        Path('content/math').mkdir(parents=True)
        Path('content/math/helper.h').write_text('inline int helper() { return 7; }')
        source = ('#include "helper.h"\n// begin template //\n'
                  'int answer() {\n return helper();\n}\n// end template //')
        prepare.generate(source, 'listing.tex', 'math/answer.cpp')
        listing = Path('listing.tex').read_text()
        self.assertIn('return helper();', listing)
        self.assertNotIn('inline int helper', listing)

    def test_cache_checks_source_and_caption_filename(self):
        prepare.save_cached_template('source', 'graph/a.cpp',
                                    'caption - graph/a.cpp - 1 lines\n')
        self.assertIsNotNone(prepare.cached_template('source', 'graph/a.cpp'))
        self.assertIsNone(prepare.cached_template('changed', 'graph/a.cpp'))
        self.assertIsNone(prepare.cached_template('source', 'graph/b.cpp'))

    def test_cache_version_invalidates_old_listings(self):
        prepare.save_cached_template('source', 'graph/a.cpp',
                                    'caption - graph/a.cpp - 1 lines\n')
        with patch.object(prepare, 'CACHE_VERSION', prepare.CACHE_VERSION + 1):
            self.assertIsNone(prepare.cached_template('source', 'graph/a.cpp'))

    @unittest.skipUnless(shutil.which('clang++') and shutil.which('cpp'),
                         'Clang and cpp required')
    def test_lambda_semicolon_is_preserved_and_hashed(self):
        for suffix in [';', '  ;', '\n;', ' /* note */ ;', ' // note\n;']:
            with self.subTest(suffix=suffix):
                source = ('// begin template //\nauto f = [](int x) {\n'
                          ' return x + 1;\n}' + suffix + '\n// end template //')
                with patch.object(prepare, 'hash', wraps=prepare.hash) as digest:
                    annotated = prepare.add_method_hashes(
                        prepare.process2(source), prepare.process(source), 'lambda.cpp')
                self.assertEqual(digest.call_count, 1)
                hashed = digest.call_args.args[0]
                self.assertTrue(hashed.endswith(suffix))
                expected = prepare.hash(hashed)
                self.assertIn('; // ' + expected, annotated)
                self.assertNotEqual(expected, prepare.hash(hashed[:-1]))
                self.assert_compiles(annotated)

    def assert_compiles(self, source):
        result = subprocess.run(
            ['clang++', '-std=c++20', '-x', 'c++', '-fsyntax-only', '-'],
            input=source, capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)

    @unittest.skipUnless(shutil.which('clang++') and shutil.which('cpp'),
                         'Clang and cpp required')
    def test_hash_comments_preserve_same_line_code(self):
        snippets = [
            'auto f = [] {\n return 7;\n}; int answer = f();',
            'int f() {\n return 7;\n} int answer = f();',
            'int answer = [] {\n return 7;\n}();',
        ]
        for snippet in snippets:
            with self.subTest(snippet=snippet):
                source = '// begin template //\n' + snippet + '\n// end template //'
                prepare.generate(source, 'listing.tex', 'lambda.cpp')
                listing = Path('listing.tex').read_text()
                annotated = listing.split('\\begin{minted}{cpp}\n')[1].split('\\end{minted}')[0]
                self.assertRegex(annotated, r'/\* [0-9a-f]{6} \*/')
                self.assert_compiles(annotated)
                self.assertEqual(prepare.hash(snippet), prepare.hash(annotated))
                self.assertIn(prepare.hash(snippet) + ' - lambda.cpp', listing)
                # An unchanged source also retains the corrected listing on cache hits.
                with patch.object(prepare.subprocess, 'run') as run:
                    prepare.generate(source, 'cached.tex', 'lambda.cpp')
                run.assert_not_called()
                self.assertEqual(Path('cached.tex').read_text(), listing)

    @unittest.skipUnless(shutil.which('clang++') and shutil.which('cpp'),
                         'Clang and cpp required')
    def test_nested_lambdas_and_enclosing_function_hash_original_code(self):
        inner = 'auto inner = [](int x) {\n return x + 1;\n};'
        outer = ('auto outer = [&](auto self, int x) -> int {\n' + inner +
                 '\n return x ? self(self, x-1) : inner(x);\n};')
        function = 'int f() {\n' + outer + '\n return outer(outer, 2);\n}'
        source = ('// comentário antes do template\n// begin template //\n'
                  '// função com lambdas aninhadas\n' + function + '\n// end template //')
        annotated = prepare.add_method_hashes(
            prepare.process2(source), prepare.process(source), 'nested.cpp')
        for body in [inner, outer, function]:
            digest = prepare.hash(body)
            self.assertEqual(annotated.count('// ' + digest), 1, annotated)
        self.assertEqual(annotated.count('// '), 4)  # Three hashes and original comment.
        self.assertEqual(prepare.hash(annotated), prepare.hash(function))
        self.assert_compiles(annotated)

    @unittest.skipUnless(shutil.which('clang++') and shutil.which('cpp'),
                         'Clang and cpp required')
    def test_lambdas_in_constructor_and_method(self):
        first = 'auto first = [] {\n return 1;\n};'
        second = 'auto second = [] {\n return 2;\n};'
        body = ('struct S {\n S() {\n' + first + '\n}\n'
                'int method() {\n' + second + '\n return second();\n}\n};')
        source = '// begin template //\n' + body + '\n// end template //'
        annotated = prepare.add_method_hashes(
            prepare.process2(source), prepare.process(source), 'members.cpp')
        for lam in [first, second]:
            self.assertIn('}; // ' + prepare.hash(lam), annotated)
        self.assertEqual(prepare.hash(annotated), prepare.hash(body))
        self.assert_compiles(annotated)

    @unittest.skipUnless(shutil.which('clang++') and shutil.which('cpp'),
                         'Clang and cpp required')
    def test_repository_bridges_lambdas_are_annotated(self):
        source_path = Path(spec.origin).resolve().parents[1] / 'content/graph/bridges.cpp'
        source = source_path.read_text()
        snippet = prepare.process(source)
        annotated = prepare.add_method_hashes(
            prepare.process2(source), snippet, str(source_path))
        self.assertEqual(annotated.count('}; // '), 2)  # make and recursive dfs.
        self.assertEqual(prepare.hash(annotated), prepare.hash(snippet))
        self.assert_compiles(source.replace(snippet, annotated))

    def test_batch_deduplicates_and_excludes_comments_and_starter(self):
        Path('content/graph').mkdir(parents=True)
        Path('content/contest').mkdir()
        Path('content/graph/a.cpp').write_text('// begin template //\nint a;')
        Path('content/contest/template.cpp').write_text('int main() {}')
        Path('content/graph/main.tex').write_text(
            '{graph/a.cpp}\n{graph/a.cpp}\n% {missing/file.cpp}\n'
            '{template.cpp}{content/contest/template.cpp}\n'
        )
        with patch.object(prepare, 'generate') as generate:
            prepare.prepare_all()
        generate.assert_called_once_with(
            '// begin template //\nint a;',
            Path('out/prepared/graph/a.cpp.tex'), 'graph/a.cpp'
        )

    def test_failed_cache_publication_preserves_previous_entry(self):
        data = 'caption - graph/a.cpp - 1 lines\n'
        prepare.save_cached_template('source', 'graph/a.cpp', data)
        with patch.object(Path, 'replace', side_effect=OSError('write failed')):
            with self.assertRaisesRegex(OSError, 'write failed'):
                prepare.save_cached_template('source', 'graph/a.cpp', 'replacement')
        self.assertEqual(prepare.cached_template('source', 'graph/a.cpp'), data)
        self.assertEqual(len(list(Path('out/template-cache').iterdir())), 1)

    def test_cached_generation_does_not_invoke_compiler(self):
        data = 'caption - graph/a.cpp - 1 lines\n'
        prepare.save_cached_template('source', 'graph/a.cpp', data)
        with patch.object(prepare.subprocess, 'run') as run:
            prepare.generate('source', 'listing.tex', 'graph/a.cpp')
        run.assert_not_called()
        self.assertEqual(Path('listing.tex').read_text(), data)

    def test_parallel_jobs_finish_and_propagate_failures(self):
        Path('content/graph').mkdir(parents=True)
        for name in ['a', 'b']:
            Path(f'content/graph/{name}.cpp').write_text('// begin template //\nint a;')
        Path('content/graph/main.tex').write_text('{graph/a.cpp}{graph/b.cpp}')
        barrier = threading.Barrier(2)

        def generate(template, output, name):
            barrier.wait(timeout=5)
            output.write_text(name)

        with patch.object(prepare, 'generate', side_effect=generate):
            prepare.prepare_all(jobs=2)
        for name in ['a', 'b']:
            self.assertEqual(Path(f'out/prepared/graph/{name}.cpp.tex').read_text(),
                             f'graph/{name}.cpp')
        with patch.object(prepare, 'generate', side_effect=RuntimeError('compiler failed')):
            with self.assertRaisesRegex(RuntimeError, 'compiler failed'):
                prepare.prepare_all(jobs=2)


if __name__ == '__main__':
    unittest.main()
