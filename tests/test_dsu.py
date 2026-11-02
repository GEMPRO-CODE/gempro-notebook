from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
COMPILER = shutil.which('g++')


@unittest.skipUnless(COMPILER, 'g++ required')
class DSUTests(unittest.TestCase):
    def test_rollback(self):
        source = r'''
#include <cassert>
#include "dsa/dsu.cpp"
int main() {
    mt19937 gen(123);
    for (int n : {1,2,17,64}) {
        DSU d(n);
        vi labels(n); iota(all(labels),0);
        vector<vi> saved;
        vector<bool> changed;
        auto check = [&] {
            for (int i=0;i<n;++i) {
                int count=0;
                for (int j=0;j<n;++j) {
                    assert((d.root(i)==d.root(j))==(labels[i]==labels[j]));
                    count += labels[i]==labels[j];
                }
                assert(d.cnt[d.root(i)]==count);
            }
        };
        auto undo = [&] {
            assert(d.rollback()==changed.back()); changed.pop_back();
            labels=saved.back(); saved.pop_back(); check();
        };
        for (int step=0;step<1500;++step) {
            if (!saved.empty() && gen()%2) undo();
            else {
                int a=gen()%n, b=gen()%n;
                saved.push_back(labels);
                bool change=labels[a]!=labels[b]; changed.push_back(change);
                assert(d.merge(a,b)==change);
                int old=labels[b], now=labels[a];
                for (int &x:labels) if (x==old) x=now;
                check();
            }
        }
        while (!saved.empty()) undo();
        assert(d.hist.empty());
        for (int i=0;i<n;++i) assert(d.par[i]==i && d.cnt[i]==1);
    }
}
'''
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory)
            (path / 'test.cpp').write_text(source)
            result = subprocess.run(
                [COMPILER, '-std=c++20', '-O2', '-fsanitize=undefined',
                 '-fno-sanitize-recover=all', '-I', str(ROOT / 'content'),
                 str(path / 'test.cpp'), '-o', str(path / 'test')],
                capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            result = subprocess.run([str(path / 'test')], capture_output=True,
                                    text=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stderr)
