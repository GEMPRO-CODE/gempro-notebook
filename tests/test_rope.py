from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
COMPILER = shutil.which('g++')


@unittest.skipUnless(COMPILER, 'GNU C++ with rope required')
class RopeTests(unittest.TestCase):
    def test_edits_and_snapshots(self):
        source = r'''
#include <cassert>
#include <random>
#include <vector>
#include "dsa/gnu-extensions.cpp"
int main() {
    ropeExample();
    std::mt19937 rng(713);
    rope<int> a;
    std::vector<int> v;
    std::vector<rope<int>> versions;
    std::vector<std::vector<int>> expected;
    auto check=[](const rope<int>& r,const std::vector<int>& v) {
        assert(r.size()==v.size());
        for(size_t i=0;i<v.size();++i) assert(r[i]==v[i]);
    };
    for(int step=0;step<3000;++step) {
        versions.push_back(a); expected.push_back(v);
        int op=rng()%4, pos=rng()%(v.size()+1);
        if(op==0) {
            int x=int(rng()); a.insert(pos,x); v.insert(v.begin()+pos,x);
        } else if(op==1 && !v.empty()) {
            pos=rng()%v.size(); int x=int(rng());
            a.replace(pos,x); v[pos]=x;
        } else {
            int len=rng()%(v.size()-pos+1);
            auto part=a.substr(pos,len);
            std::vector<int> cut(v.begin()+pos,v.begin()+pos+len);
            a.erase(pos,len); v.erase(v.begin()+pos,v.begin()+pos+len);
            if(op==2) {
                int dest=rng()%(v.size()+1);
                a.insert(dest,part); v.insert(v.begin()+dest,cut.begin(),cut.end());
            }
            check(part,cut);
        }
        check(a,v);
        int saved=rng()%versions.size(); check(versions[saved],expected[saved]);
    }
    for(size_t i=0;i<versions.size();++i) check(versions[i],expected[i]);
    crope s("abc"); auto saved=s; s+="def";
    assert(s==crope("abcdef") && saved==crope("abc"));
}
'''
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory)
            (path / 'test.cpp').write_text(source)
            result = subprocess.run(
                [COMPILER, '-std=c++20', '-O2', '-Wall', '-Wextra',
                 '-fsanitize=undefined', '-fno-sanitize-recover=all',
                 '-I', str(ROOT / 'content'), str(path / 'test.cpp'),
                 '-o', str(path / 'test')], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            result = subprocess.run([str(path / 'test')], capture_output=True,
                                    text=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stderr)
