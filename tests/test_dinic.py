import json
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest


COMPILER = shutil.which('g++') or shutil.which('clang++')


@unittest.skipUnless(COMPILER, 'C++ compiler required')
class DinicTests(unittest.TestCase):
    def test_limits_resume_and_exhaustive_min_cuts(self):
        snippet = Path(__file__).resolve().parents[1] / 'content/graph/dinic.cpp'
        source = '#define main notebook_main\n#include ' + json.dumps(str(snippet))
        source += r'''
#undef main
#include <cassert>
#include <tuple>
int main() {
    Dinic g(2);
    g.addEdge(0, 1, 10);
    assert(g.flow(0, 1, 0) == 0);
    assert(g.flow(0, 1, 3) == 3);
    assert(g.flow(0, 1, 2) == 2);
    assert(g.flow(0, 1) == 5);
    assert(g.flow(0, 1) == 0);
    mt19937 random(2026);
    for (int trial = 0; trial < 100; ++trial) {
        int n = 2 + random() % 6;
        Dinic a(n), b(n);
        vector<tuple<int, int, ll>> edges;
        for (int i = 0; i < 20; ++i) {
            int u = random() % n, v = random() % n;
            ll cap = random() % 10;
            if (u == v) continue;
            edges.emplace_back(u, v, cap);
            a.addEdge(u, v, cap);
            b.addEdge(u, v, cap);
        }
        ll cut = LLONG_MAX;
        for (int mask = 0; mask < (1 << n); ++mask) {
            if (!(mask & 1) || (mask & (1 << (n - 1)))) continue;
            ll cost = 0;
            for (auto [u, v, cap] : edges)
                if ((mask & (1 << u)) && !(mask & (1 << v))) cost += cap;
            cut = min(cut, cost);
        }
        ll limit = random() % 20;
        ll first = a.flow(0, n - 1, limit);
        assert(first == min(limit, cut));
        assert(first + a.flow(0, n - 1) == cut);
        assert(b.flow(0, n - 1) == cut);
    }
}
'''
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / 'test.cpp').write_text(source)
            subprocess.run([COMPILER, '-std=c++20', '-O2', str(root / 'test.cpp'),
                            '-o', str(root / 'test')], check=True, capture_output=True)
            subprocess.run([str(root / 'test')], check=True, timeout=10,
                           capture_output=True)


if __name__ == '__main__':
    unittest.main()
