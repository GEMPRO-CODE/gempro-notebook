from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
COMPILER = shutil.which('g++')


@unittest.skipUnless(COMPILER, 'g++ required')
class ReconstructionTests(unittest.TestCase):
    def run_cpp(self, file, body, small_field=False):
        snippet = (ROOT / 'content' / file).read_text().split('int main()')[0]
        if small_field:
            snippet = snippet.replace('const int mod = 998244353;', 'const int mod = 5;')
        source = '#include <bits/stdc++.h>\n#include <cassert>\n' + snippet + body
        with tempfile.TemporaryDirectory() as directory:
            p = Path(directory)
            (p / 'test.cpp').write_text(source)
            result = subprocess.run(
                [COMPILER, '-std=c++20', '-O2', '-fsanitize=undefined',
                 '-fno-sanitize-recover=all', str(p / 'test.cpp'), '-o', str(p / 'test')],
                capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            result = subprocess.run([str(p / 'test')], capture_output=True,
                                    text=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stderr)

    def test_sam_occurrence_positions(self):
        self.run_cpp('strings/suffix-automaton.cpp', r'''
int main() {
    mt19937 gen(317);
    for(int it=0;it<500;++it) {
        string s; SAM sam;
        assert(sam.t[0].end==-1);
        for(int n=0;n<25;++n) {
            s+=char('a'+gen()%3); sam.extend(s.back());
            vector<string> representative(sam.t.size());
            for(int l=0;l<int(s.size());++l) {
                int v=0;
                for(int r=l;r<int(s.size());++r) {
                    v=sam.t[v].to[s[r]-'a']; assert(v>0);
                    int len=r-l+1, end=sam.t[v].end;
                    assert(end>=len-1 && end<int(s.size()));
                    assert(s.substr(end-len+1,len)==s.substr(l,len));
                    if(len==sam.t[v].len) representative[v]=s.substr(l,len);
                }
            }
            for(int v=1;v<int(sam.t.size());++v)
                assert(int(representative[v].size())==sam.t[v].len);
        }
    }
}
''')

    def test_linear_system_all_solutions(self):
        self.run_cpp('math/gaussian-elimination.cpp', r'''
int main() {
    mt19937 gen(318);
    for(int n=0;n<=4;++n) for(int m=0;m<=4;++m)
    for(int it=0;it<60;++it) {
        vector<vector<ll>> a(n,vector<ll>(m+1));
        for(auto &r:a) for(auto &v:r) v=gen()%mod;
        auto original=a;
        vector<ll> x; vector<vector<ll>> basis;
        bool ok=linearSystem(a,m,x,basis);
        assert(a==original);
        set<vector<ll>> want,got;
        int total=1; for(int j=0;j<m;++j) total*=mod;
        for(int mask=0;mask<total;++mask) {
            int z=mask; vector<ll> v(m);
            for(auto &y:v) y=z%mod,z/=mod;
            bool valid=true;
            for(auto &r:a) {
                ll sum=0; for(int j=0;j<m;++j) sum+=r[j]*v[j];
                valid &= sum%mod==r[m];
            }
            if(valid) want.insert(v);
        }
        assert(ok==!want.empty()); if(!ok) continue;
        assert(x.size()==size_t(m));
        int count=1; for(auto &v:basis) {
            assert(v.size()==size_t(m)); count*=mod;
        }
        for(int mask=0;mask<count;++mask) {
            auto v=x; int z=mask;
            for(auto &b:basis) {
                int c=z%mod; z/=mod;
                for(int j=0;j<m;++j) v[j]=(v[j]+c*b[j])%mod;
            }
            assert(got.insert(v).second);
        }
        assert(got==want);
    }
}
''', small_field=True)
