import json
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
COMPILER = shutil.which('g++') or shutil.which('clang++')


@unittest.skipUnless(COMPILER, 'C++ compiler required')
class ConsolidationTests(unittest.TestCase):
    def run_cpp(self, path, body, maximum=False):
        snippet = (ROOT / 'content' / path).read_text()
        if maximum:
            snippet = snippet.replace('LLONG_MAX','LLONG_MIN')
            for expression in ['line(tm) < t[i](tm)', 't[i](tl) < line(tl)', 't[i](x) < line(x)']:
                snippet = snippet.replace(expression, expression.replace(' < ', ' > '))
        source = '#include <cassert>\n#include <algorithm>\n#include <climits>\n'
        source += '#define main notebook_main\n' + snippet + '\n#undef main\n' + body
        with tempfile.TemporaryDirectory() as directory:
            p = Path(directory)
            (p/'test.cpp').write_text(source)
            result = subprocess.run([COMPILER,'-std=c++20','-O2','-iquote',str((ROOT/'content'/path).parent),str(p/'test.cpp'),'-o',str(p/'test')],capture_output=True,text=True)
            self.assertEqual(result.returncode,0,result.stderr)
            result = subprocess.run([str(p/'test')],capture_output=True,text=True,timeout=30)
            self.assertEqual(result.returncode,0,result.stderr)

    def test_li_chao_full_and_partial_domains(self):
        body = r'''
int main() {
    mt19937 gen(42);
    for(int n: {1,2,3,17,64}) for(int trial=0;trial<20;++trial) {
        LiChao tree(n);vector<ll> expected(n,LLONG_MAX);
        for(int x=0;x<n;++x)assert(tree.query(x)(x)==expected[x]);
        for(int i=0;i<200;++i){
            Line line{(int)(gen()%101)-50,(int)(gen()%101)-50};
            int l=gen()%(n+1),r=gen()%(n+1);if(l>r)swap(l,r);
            if(i%3==0){tree.insert(line);l=0;r=n;}else tree.insert(line,l,r);
            for(int x=l;x<r;++x)expected[x]=min(expected[x],line(x));
            for(int x=0;x<n;++x)assert(tree.query(x)(x)==expected[x]);
        }
    }
}
'''
        self.run_cpp('dsa/li-chao-extended.cpp',body)
        self.run_cpp('dsa/li-chao-extended.cpp',body.replace('LLONG_MAX','LLONG_MIN').replace('=min(', '=max('),maximum=True)

    def test_wavelet_queries_and_position_mapping(self):
        self.run_cpp('dsa/wavelet-matrix-extended.cpp',r'''
int main() {
    mt19937 gen(42);const ll limit=1LL<<H;
    for(int n:{0,1,2,17,63})for(int trial=0;trial<25;++trial){
        vector<ll>a(n),weights(n);for(int i=0;i<n;++i){
            a[i]=trial%2?gen()%8:gen()%limit;weights[i]=(int)(gen()%101)-50;
        }
        if(n>1){a[0]=0;a[1]=limit-1;}
        auto wm=wavMat(a);auto pos=wavPos(wm);
        vi order(n);iota(all(order),0);WM ids;
        vector<vector<ll>> sums(H,vector<ll>(n+1));
        for(int h=H-1;h>=0;--h){
            stable_partition(all(order),[&](int i){return !(a[i]>>h&1);});ids[h]=order;
            for(int j=0;j<n;++j){assert(pos[h][order[j]]==j);sums[h][j+1]=sums[h][j]+weights[order[j]];}
        }
        for(int t=0;t<150;++t){
            int l=gen()%(n+1),r=gen()%(n+1);if(l>r)swap(l,r);
            ll lo=gen()%(limit+1),hi=gen()%(limit+1);if(lo>hi)swap(lo,hi);
            if(t%4==0){lo=0;hi=limit;}
            if(t%7==0)lo=hi;
            vi seen(n);ll got=0,want=0;int calls=0;
            rectOp(wm,l,r,lo,hi,[&](int h,int L,int R){
                assert(0<=h&&h<H&&0<=L&&L<R&&R<=n);++calls;
                got+=sums[h][R]-sums[h][L];
                for(int j=L;j<R;++j)++seen[ids[h][j]];
            });
            assert(calls<=2*H);
            for(int i=0;i<n;++i){bool inside=l<=i&&i<r&&lo<=a[i]&&a[i]<hi;
                assert(seen[i]==inside);if(inside)want+=weights[i];}
            assert(got==want);
            vector<ll> sorted(a.begin()+l,a.begin()+r);sort(all(sorted));
            for(int k=0;k<sz(sorted);++k)assert(rangeKth(wm,H,l,r,k)==sorted[k]);
            ll x=hi%limit;int count=0;for(ll v:sorted)count+=v<x;
            assert(cntLess(wm,H,l,r,x)==count);
        }
    }
}
''')


if __name__ == '__main__':
    unittest.main()
