from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
COMPILER = shutil.which('g++') or shutil.which('clang++')


@unittest.skipUnless(COMPILER, 'C++ compiler required')
class LibraryMoreTests(unittest.TestCase):
    def run_cpp(self, name, body):
        source = '#include <bits/stdc++.h>\n#include <cassert>\n'
        source += f'#include "{name}.cpp"\n' + body
        with tempfile.TemporaryDirectory() as directory:
            p = Path(directory)
            (p / 'test.cpp').write_text(source)
            result = subprocess.run(
                [COMPILER, '-std=c++20', '-O2', '-fsanitize=undefined',
                 '-fno-sanitize-recover=all', '-I', str(ROOT / 'content'),
                 str(p / 'test.cpp'), '-o', str(p / 'test')],
                capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            result = subprocess.run([str(p / 'test')], capture_output=True,
                                    text=True, timeout=90)
            self.assertEqual(result.returncode, 0, result.stderr)

    def test_triangles(self):
        self.run_cpp('graph/triangles', r'''
void check(vector<vi> &g){
    int n=g.size();vector<vector<bool>>adj(n,vector<bool>(n));
    for(int u=0;u<n;++u)for(int v:g[u])adj[u][v]=true;
    set<array<int,3>>want,got;
    for(int a=0;a<n;++a)for(int b=a+1;b<n;++b)for(int c=b+1;c<n;++c)
        if(adj[a][b]&&adj[a][c]&&adj[b][c])want.insert({a,b,c});
    triangles(g,[&](int a,int b,int c){
        assert((pair{g[a].size(),a}<pair{g[b].size(),b}));
        assert((pair{g[b].size(),b}<pair{g[c].size(),c}));
        array<int,3>x{a,b,c};sort(x.begin(),x.end());
        assert(got.insert(x).second);
    });assert(got==want);
}
int main(){
    for(int n=0;n<=6;++n)for(int mask=0;mask<(1<<(n*(n-1)/2));++mask){
        vector<vi>g(n);int k=0;
        for(int u=0;u<n;++u)for(int v=u+1;v<n;++v,++k)if(mask>>k&1)
            g[u].push_back(v),g[v].push_back(u);
        check(g);
    }
    mt19937 gen(721);
    for(int it=0;it<300;++it){int n=1+gen()%60;vector<vi>g(n);
        for(int u=0;u<n;++u)for(int v=u+1;v<n;++v)if(gen()%3==0)
            g[u].push_back(v),g[v].push_back(u);
        for(auto&v:g)shuffle(v.begin(),v.end(),gen);check(g);
    }
    vector<vi>star(200000);int count=0;
    for(int i=1;i<int(star.size());++i)star[0].push_back(i),star[i].push_back(0);
    triangles(star,[&](int,int,int){++count;});assert(count==0);
    int n=250;vector<vi>clique(n);
    for(int u=0;u<n;++u)for(int v=0;v<n;++v)if(u!=v)clique[u].push_back(v);
    triangles(clique,[&](int,int,int){++count;});assert(count==n*(n-1)*(n-2)/6);
}
''')

    def test_range_unionfind(self):
        self.run_cpp('dsa/range-unionfind', r'''
int main(){mt19937 gen(722);
    for(int trial=0;trial<150;++trial){
        int n=1+gen()%40;RangeUnionFind ds(n);
        vi p(n);iota(p.begin(),p.end(),0);
        auto root=[&](int x){while(x!=p[x])x=p[x];return x;};
        vector<long long>w(n),sum(n);for(auto&x:w)x=1+gen()%20;sum=w;
        long long ans=0;int merges=0;
        auto f=[&](int x,int y){
            assert(x!=y&&ds.root(x)==x&&ds.root(y)==x);
            ans+=sum[x]*sum[y];sum[x]+=sum[y];sum[y]=0;++merges;
        };
        for(int step=0;step<150;++step){
            int len=gen()%(n+1),a=gen()%(n-len+1),b=gen()%(n-len+1);
            ds.merge(a,b,len,f);
            for(int i=0;i<len;++i)p[root(a+i)]=root(b+i);
            long long expected=0;vector<long long>tot(n);set<int>components;
            for(int i=0;i<n;++i){tot[ds.root(i)]+=w[i];components.insert(root(i));
                for(int j=0;j<i;++j){
                    assert((ds.root(i)==ds.root(j))==(root(i)==root(j)));
                    if(root(i)==root(j))expected+=w[i]*w[j];
                }
            }
            assert(ans==expected&&sum==tot&&merges==n-int(components.size()));
            // Idempotence, including empty and overlapping intervals.
            ds.merge(a,b,len,f);assert(ans==expected);
        }
    }
    int n=200000,count=0;RangeUnionFind ds(n);
    auto f=[&](int,int){++count;};
    ds.merge(0,1,n-1,f);assert(count==n-1);
    for(int q=0;q<200000;++q){
        int len=1+gen()%n,a=gen()%(n-len+1),b=gen()%(n-len+1);
        ds.merge(a,b,len,f);
    }
    assert(count==n-1);
    for(int i=0;i<n;++i)assert(ds.root(i)==ds.root(0));
}
''')

    def test_min_mod(self):
        self.run_cpp('math/min-mod', r'''
using I=__int128_t;
ll checkMinMod(ll n,ll m,ll a,ll b){
    auto [value,i]=minMod(n,m,a,b);
    assert(0<=i && i<n && i<m/gcd(a,m));
    assert((I(a)*i+b)%m==value);
    return value;
}
// Independent oracle: count residues below a threshold using floor sums.
I floors(I n,I m,I a,I b){
    I ans=0;
    while(true){
        ans+=(n-1)*n*(a/m)/2+n*(b/m);a%=m;b%=m;
        I y=a*n+b;if(y<m)return ans;
        n=y/m;b=y%m;swap(a,m);
    }
}
ll oracle(ll n,ll m,ll a,ll b){
    a%=m;b%=m;ll l=0,r=m-1;I base=floors(n,m,a,b);
    while(l<r){ll mid=l+(r-l)/2;
        I greater=floors(n,m,a,I(b)+m-1-mid)-base;
        if(greater<n)r=mid;else l=mid+1;
    }return l;
}
int main(){
    for(ll m=1;m<=60;++m)for(ll a=0;a<m;++a)for(ll b=0;b<m;++b){
        ll best=m;
        for(ll n=1;n<=90;++n){
            best=min(best,(a*(n-1)+b)%m);
            assert(checkMinMod(n,m,a,b)==best);
        }
    }
    mt19937_64 gen(723);
    for(int it=0;it<10000;++it){
        ll n=1+gen()%100,m=1+(gen()&LLONG_MAX)%LLONG_MAX;
        ll a=gen()&LLONG_MAX,b=gen()&LLONG_MAX,best=m;
        for(ll i=0;i<n;++i)best=min(best,ll((I(a)*i+b)%m));
        assert(checkMinMod(n,m,a,b)==best);
    }
    for(int it=0;it<500;++it){
        ll n=1+(gen()&LLONG_MAX)%LLONG_MAX;
        ll m=1+(gen()&LLONG_MAX)%LLONG_MAX;
        ll a=gen()&LLONG_MAX,b=gen()&LLONG_MAX;
        assert(checkMinMod(n,m,a,b)==oracle(n,m,a,b));
        assert(checkMinMod(LLONG_MAX,m,a,b)==b%gcd(a,m));
    }
    assert(checkMinMod(LLONG_MAX,LLONG_MAX,LLONG_MAX-1,LLONG_MAX-2)==0);
    assert(checkMinMod(1,LLONG_MAX,LLONG_MAX,LLONG_MAX)==0);
    assert(checkMinMod(LLONG_MAX,1,LLONG_MAX,LLONG_MAX)==0);
    assert(checkMinMod(LLONG_MAX,LLONG_MAX,0,LLONG_MAX-1)==LLONG_MAX-1);
}
''')


if __name__ == '__main__':
    unittest.main()
