import json
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
COMPILER = shutil.which('g++') or shutil.which('clang++')


@unittest.skipUnless(COMPILER, 'C++ compiler required')
class HavelHakimiTests(unittest.TestCase):
    def test_realizes_graphical_sequences(self):
        source = r'''
#include <algorithm>
#include <cassert>
#define main snippet_main
#include SNIPPET
#undef main
void check(vi degrees) {
    auto original = degrees;
    auto edges = havelHakimi(degrees);
    assert(degrees == original);
    int n = degrees.size();
    vector<vi> seen(n,vi(n));
    vi got(n);
    for (auto [u,v] : edges) {
        assert(0 <= u && u < n && 0 <= v && v < n && u != v);
        assert(!seen[u][v]); seen[u][v] = seen[v][u] = 1;
        ++got[u]; ++got[v];
    }
    assert(got == degrees);
}
int main() {
    check({2,2,2}); // Regression: rem(u) used to declare a variable.
    for (int n = 0; n <= 6; ++n) {
        int m = n*(n-1)/2;
        for (int mask = 0; mask < (1<<m); ++mask) {
            vi d(n); int bit = 0;
            for (int u = 0; u < n; ++u) for (int v = u+1; v < n; ++v,++bit)
                if (mask>>bit&1) ++d[u], ++d[v];
            check(d);
        }
    }
    mt19937 gen(2017);
    for (int trial = 0; trial < 2000; ++trial) {
        int n = 1+gen()%100; vi d(n);
        for (int u = 0; u < n; ++u) for (int v = u+1; v < n; ++v) {
            bool edge = gen()%100 < (unsigned)(trial%101);
            if (trial%5 == 0) edge = u == 0;
            if (trial%7 == 0) edge = (u < n/2) != (v < n/2);
            if (edge) ++d[u], ++d[v];
        }
        shuffle(d.begin(),d.end(),gen); check(d);
    }
}
'''.replace('SNIPPET', json.dumps(str(ROOT / 'content/graph/havel-hakimi.cpp')))
        with tempfile.TemporaryDirectory() as directory:
            p = Path(directory)
            (p / 'test.cpp').write_text(source)
            result = subprocess.run([COMPILER, '-std=c++20', '-O2', '-fsanitize=undefined',
                                     str(p / 'test.cpp'), '-o', str(p / 'test')],
                                    capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            result = subprocess.run([str(p / 'test')], capture_output=True, text=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertEqual(result.stderr, '')


if __name__ == '__main__':
    unittest.main()
