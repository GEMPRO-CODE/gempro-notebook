import json
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
COMPILER = shutil.which('g++') or shutil.which('clang++')


@unittest.skipUnless(COMPILER, 'C++ compiler required')
class ShortenedSnippetTests(unittest.TestCase):
    def run_cpp(self, path, body):
        source = '#include <algorithm>\n#include <cassert>\n#include <climits>\n#include <string>\n'
        source += '#define main notebook_main\n#include '+json.dumps(str(ROOT/'content'/path))+'\n#undef main\n'+body
        with tempfile.TemporaryDirectory() as directory:
            p = Path(directory)
            (p/'test.cpp').write_text(source)
            result = subprocess.run([COMPILER,'-std=c++20','-O2',str(p/'test.cpp'),'-o',str(p/'test')],capture_output=True,text=True)
            self.assertEqual(result.returncode,0,result.stderr)
            result = subprocess.run([str(p/'test')],capture_output=True,text=True,timeout=30)
            self.assertEqual(result.returncode,0,result.stderr)

    def test_floor_moments_against_direct_sums(self):
        self.run_cpp('math/floor-sum.cpp',r'''
void check(ll n,ll m,ll a,ll b) {
    array<i128,3> want{};
    for(ll i=0;i<n;++i){i128 x=((i128)a*i+b)/m;want[0]+=x;want[1]+=i*x;want[2]+=x*x;}
    assert(floorSums(n,m,a,b)==want);
    assert(floorSum(n,m,a,b)==want[0]);
}
int main(){
    for(ll n=0;n<20;++n)for(ll m=1;m<20;++m)
    for(ll a=0;a<20;++a)for(ll b=0;b<20;++b)check(n,m,a,b);
    mt19937_64 gen(2026);
    for(int t=0;t<2000;++t)check(gen()%100,1+gen()%100000,gen()%1000000000000LL,gen()%1000000000000LL);
    assert(floorSum(LLONG_MAX,1,0,1)==LLONG_MAX);
    assert(floorSum(LLONG_MAX,LLONG_MAX,1,0)==0);
    ll n=1000000000;auto [x,y,z]=floorSums(n,1,1,0);
    assert(x==(i128)n*(n-1)/2);
    assert(y==(i128)n*(n-1)*(2*n-1)/6&&z==y);
}
''')

    def test_subset_transforms_and_inverse_boundaries(self):
        self.run_cpp('dsa/zeta.cpp',r'''
int main(){
    assert(subsetZeta(vi{INT_MIN,INT_MIN},true)==vi({INT_MIN,0}));
    assert(supersetZeta(vi{INT_MIN,INT_MIN},true)==vi({0,INT_MIN}));
    assert(divZeta(vi{0,INT_MIN,INT_MIN},true)==vi({0,INT_MIN,0}));
    assert(mulZeta(vi{0,INT_MIN,INT_MIN},true)==vi({0,0,INT_MIN}));
    mt19937 gen(42);
    for(int bits=0;bits<=7;++bits)for(int t=0;t<50;++t){
        int n=1<<bits;vi a(n),sub(n),sup(n);
        for(int &x:a)x=(int)(gen()%101)-50;
        for(int i=0;i<n;++i)for(int j=0;j<n;++j){
            if((i&j)==j)sub[i]+=a[j];
            if((i&j)==i)sup[i]+=a[j];
        }
        assert(subsetZeta(a)==sub&&supersetZeta(a)==sup);
        assert(subsetZeta(sub,true)==a&&supersetZeta(sup,true)==a);
    }
}
''')

    def test_sparse_table_empty_rebuild_and_noncommutative_op(self):
        self.run_cpp('dsa/sparse-table.cpp',r'''
int main(){
    SparseTable<int, [](int a,int b){return min(a,b);}> st;
    SparseTable<string, [](string a,string b){return a;}> left;
    mt19937 gen(42);
    for(int n:{0,1,2,3,7,8,9,32,65,0,2}){
        vi v(n);vector<string>w(n);
        for(int i=0;i<n;++i)v[i]=gen()%1000,w[i]=to_string(i);
        st.build(v);left.build(w);
        for(int l=0;l<n;++l){int mn=INT_MAX;for(int r=l+1;r<=n;++r){
            mn=min(mn,v[r-1]);assert(st.query(l,r)==mn);
            assert(left.query(l,r)==w[l]);
        }}
    }
}
''')

    def test_lca_single_vertex_and_parent_walk(self):
        self.run_cpp('tree/lca.cpp',r'''
int main(){
    mt19937 gen(42);
    for(int n=1;n<=70;++n){
        vector<vi>g(n);vi parent(n),depth(n);
        for(int i=1;i<n;++i){parent[i]=gen()%i;depth[i]=depth[parent[i]]+1;
            g[i].pb(parent[i]);g[parent[i]].pb(i);}
        LCA lca(g);
        for(int i=0;i<n;++i)for(int j=0;j<n;++j){
            int a=i,b=j;while(depth[a]>depth[b])a=parent[a];
            while(depth[b]>depth[a])b=parent[b];
            while(a!=b)a=parent[a],b=parent[b];
            assert(lca.lca(i,j)==a);
        }
    }
}
''')

    def test_aho_corasick_occurrences_and_link_order(self):
        self.run_cpp('strings/aho-corasick.cpp',r'''
int main(){
    mt19937 gen(42);
    for(int trial=0;trial<500;++trial){
        AC ac;vector<string>p;vi terminal;
        int count=trial%30;
        for(int i=0;i<count;++i){string s;
            for(int j=0,n=1+gen()%8;j<n;++j)s+='a'+gen()%3;
            if(i&&i%4==0)s=p.back();p.pb(s);terminal.pb(ac.add(p.back()));}
        ac.build();assert(sz(ac.ord)==sz(ac.t)-1);
        vi order(sz(ac.t),-1);
        for(int i=0;i<sz(ac.ord);++i){int v=ac.ord[i];assert(v&&order[v]==-1);
            order[v]=i;assert(order[ac.t[v].link]<i);}
        string text;for(int i=0;i<trial%100;++i)text+='a'+gen()%3;
        vi occurrences(sz(ac.t));int v=0;
        for(char c:text){v=ac.next(v,c);++occurrences[v];}
        for(int i=sz(ac.ord)-1;i>=0;--i){v=ac.ord[i];occurrences[ac.t[v].link]+=occurrences[v];}
        for(int i=0;i<count;++i){int want=0;
            for(int j=0;j+sz(p[i])<=sz(text);++j)want+=text.compare(j,sz(p[i]),p[i])==0;
            assert(occurrences[terminal[i]]==want);}
    }
}
''')


if __name__ == '__main__':
    unittest.main()
