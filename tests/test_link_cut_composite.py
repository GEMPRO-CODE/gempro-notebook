from collections import deque
from pathlib import Path
import random
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
COMPILER = shutil.which('g++') or shutil.which('clang++')
MOD = 998244353


@unittest.skipUnless(COMPILER, 'C++ compiler required')
class PathCompositeTests(unittest.TestCase):
    def test_library_checker_protocol_against_bfs(self):
        rng = random.Random(81276)
        with tempfile.TemporaryDirectory() as directory:
            exe = str(Path(directory) / 'composite')
            subprocess.run([COMPILER, '-std=c++20', '-O2', '-fsanitize=undefined',
                            str(ROOT / 'content/tree/link-cut.cpp'), '-o', exe],
                           check=True, capture_output=True)
            for n in [1, 2, 9, 35]:
                values = [(rng.randrange(1, MOD), rng.randrange(MOD)) for _ in range(n)]
                initial = values.copy()
                edges = [(i, rng.randrange(i)) for i in range(1, n)]
                initial_edges = edges.copy()
                graph = [set() for _ in range(n)]
                for u, v in edges:
                    graph[u].add(v)
                    graph[v].add(u)

                def parents(u):
                    par = {u: -1}
                    queue = deque([u])
                    while queue:
                        v = queue.popleft()
                        for w in graph[v]:
                            if w not in par:
                                par[w] = v
                                queue.append(w)
                    return par

                queries, expected = [], []
                for step in range(1500):
                    kind = rng.randrange(3) if n > 1 else rng.randrange(1, 3)
                    if kind == 0:
                        i = rng.randrange(len(edges))
                        u, v = edges[i]
                        graph[u].remove(v)
                        graph[v].remove(u)
                        component = parents(u)
                        w = rng.choice(list(component))
                        x = rng.choice([j for j in range(n) if j not in component])
                        graph[w].add(x)
                        graph[x].add(w)
                        edges[i] = (w, x)
                        queries.append(f'0 {u} {v} {w} {x}')
                    elif kind == 1:
                        u = rng.randrange(n)
                        a, b = rng.randrange(1, MOD), rng.randrange(MOD)
                        values[u] = (a, b)
                        queries.append(f'1 {u} {a} {b}')
                    else:
                        u, v = rng.randrange(n), rng.randrange(n)
                        # Test both directions; affine composition is not commutative.
                        for start, end in [(u, v), (v, u), (u, u)]:
                            par = parents(start)
                            path, w = [], end
                            while w != -1:
                                path.append(w)
                                w = par[w]
                            x = rng.choice([0, MOD - 1, rng.randrange(MOD)])
                            queries.append(f'2 {start} {end} {x}')
                            for w in reversed(path):
                                a, b = values[w]
                                x = (a * x + b) % MOD
                            expected.append(x)
                data = [f'{n} {len(queries)}']
                data += [f'{a} {b}' for a, b in initial]
                data += [f'{u} {v}' for u, v in initial_edges]
                result = subprocess.run([exe], input='\n'.join(data + queries) + '\n',
                                        capture_output=True, text=True, timeout=20)
                self.assertEqual(result.returncode, 0, result.stderr)
                self.assertEqual(result.stderr, '')
                self.assertEqual(list(map(int, result.stdout.split())), expected)
