import json
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
COMPILER = shutil.which('g++') or shutil.which('clang++')


@unittest.skipUnless(COMPILER, 'C++ compiler required')
class NewCapabilitiesTests(unittest.TestCase):
    def run_cpp(self, path, body):
        source = ''.join('#include <' + h + '>\n' for h in ['algorithm','array','cassert','climits','cmath','numeric','optional','random','tuple','utility','vector'])
        if path == 'tree/link-cut.cpp':
            # The standalone judge example follows the printed template.
            source += (ROOT / 'content' / path).read_text().split('// end template //')[0] + '\n' + body
        else:
            source += '#include ' + json.dumps(str(ROOT / 'content' / path)) + '\n' + body
        with tempfile.TemporaryDirectory() as directory:
            p = Path(directory)
            (p / 'test.cpp').write_text(source)
            result = subprocess.run([COMPILER, '-std=c++20', '-O2', '-fsanitize=undefined',
                                     str(p / 'test.cpp'), '-o', str(p / 'test')],
                                    capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            result = subprocess.run([str(p / 'test')], capture_output=True, text=True, timeout=90)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertEqual(result.stderr, '')

    def test_summatory_against_independent_sieve(self):
        self.run_cpp('math/summatory-functions.cpp', r'''
int main() {
    const int N=1000000;
    vector<ll> phi(N+1),mu(N+1,1);iota(phi.begin(),phi.end(),0);
    for(int p=2;p<=N;++p)if(phi[p]==p){
        for(int j=p;j<=N;j+=p)phi[j]-=phi[j]/p,mu[j]=-mu[j];
        if((ll)p*p<=N)for(int j=p*p;j<=N;j+=p*p)mu[j]=0;
    }
    mu[0]=0;for(int i=1;i<=N;++i)phi[i]+=phi[i-1],mu[i]+=mu[i-1];
    for(int b:{1,2,17,1000,100000}){
        Summatory s(b);
        for(int n=0;n<=2000;++n){auto r=s.sum(n);assert(r[0]==phi[n]&&r[1]==mu[n]);}
        mt19937 gen(713);for(int i=0;i<200;++i){int n=gen()%(N+1);
            auto r=s.sum(n);assert(r[0]==phi[n]&&r[1]==mu[n]);}
        auto r=s.sum(N);assert(r[0]==phi[N]&&r[1]==mu[N]);
    }
    // Check the defining divisor-convolution identities beyond the sieve.
    Summatory s(100000);
    for(ll n:{10000000LL,100000000LL}){
        array<i128,2> total{};
        for(ll l=1,r;l<=n;l=r+1){r=n/(n/l);auto a=s.sum(r),b=s.sum(l-1);
            for(int j=0;j<2;++j)total[j]+=(a[j]-b[j])*(n/l);}
        assert(total[0]==i128(n)*(n+1)/2&&total[1]==1);
    }
}
''')

    def test_mean_cycle_against_enumerated_simple_cycles(self):
        self.run_cpp('graph/min-mean-cycle.cpp', r'''
void check(int n,vector<tuple<int,int,ll>> edges){
    vector<vector<pair<int,ll>>> g(n);for(auto [u,v,w]:edges)g[u].push_back({v,w});
    optional<pair<i128,int>> want;
    auto less=[](auto a,auto b){return a.first*b.second<b.first*a.second;};
    for(int start=0;start<n;++start){
        auto dfs=[&](auto&& self,int u,int mask,i128 weight,int length)->void{
            for(auto [v,w]:g[u]){
                if(v==start){pair<i128,int> f={weight+w,length+1};if(!want||less(f,*want))want=f;}
                else if(!(mask>>v&1))self(self,v,mask|(1<<v),weight+w,length+1);
            }
        };dfs(dfs,start,1<<start,0,0);
    }
    auto got=minMeanCycle(n,edges);assert(bool(got)==bool(want));
    if(got){assert(got->second>0);assert(!less(*got,*want)&&!less(*want,*got));}
}
int main(){
    for(int n=0;n<=4;++n)for(int mask=0;mask<(1<<(n*n));++mask){
        vector<tuple<int,int,ll>> es;for(int u=0;u<n;++u)for(int v=0;v<n;++v)
            if(mask>>(u*n+v)&1)es.emplace_back(u,v,((u*11+v*7+mask)%17)-8);check(n,es);
    }
    mt19937 gen(991);for(int trial=0;trial<1500;++trial){int n=1+gen()%7;
        vector<tuple<int,int,ll>> es;for(int i=0;i<18;++i){
            ll w=(int)(gen()%101)-50;if(trial%5==0)w=(gen()%2?LLONG_MIN:LLONG_MAX);
            es.emplace_back(gen()%n,gen()%n,w);}
        check(n,es);
    }
    check(4,{{0,1,LLONG_MIN},{1,2,LLONG_MAX},{2,3,LLONG_MIN}});
}
''')

    def test_link_cut_against_bfs_forest(self):
        self.run_cpp('tree/link-cut.cpp', r'''
ll add(ll a,ll b){return a+b;}
ll zero(){return 0;}
int main(){
    mt19937 gen(8472);
    for(int trial=0;trial<60;++trial){
        int n=1+gen()%60;LinkCut<ll,add,zero> t(n);vector<vector<int>> g(n+1,vector<int>(n+1));vector<ll> val(n+1);
        auto path=[&](int u,int v){vector<int> p(n+1,-1),q{u};p[u]=0;
            for(int i=0;i<(int)q.size();++i)for(int y=1;y<=n;++y)if(g[q[i]][y]&&p[y]<0)p[y]=q[i],q.push_back(y);
            vector<int> ans;if(p[v]>=0)for(int y=v;y;y=p[y])ans.push_back(y);return ans;};
        for(int step=0;step<5000;++step){
            int u=1+gen()%n,v=1+gen()%n,op=gen()%6;auto p=path(u,v);bool conn=!p.empty();
            if(op==0){assert(t.link(u,v)==!conn);if(!conn)g[u][v]=g[v][u]=1;}
            if(op==1){bool edge=g[u][v];assert(t.cut(u,v)==edge);g[u][v]=g[v][u]=0;}
            if(op==2){val[u]=(ll)(gen()%2000000001)-1000000000;t.set(u,val[u]);}
            if(op==3)assert(t.connected(u,v)==conn);
            if(op==4)t.makeRoot(u);
            if(op==5&&conn){ll sum=0;for(int x:p)sum+=val[x];assert(t.query(u,v)==sum);}
            if(step%100==0)for(int x=1;x<=n;++x){assert(t.query(x,x)==val[x]);assert(t.connected(u,x)==!path(u,x).empty());}
        }
    }
    const int N=100000;LinkCut<ll,add,zero> t(N);
    for(int i=1;i<=N;++i){t.set(i,i);if(i>1)assert(t.link(i-1,i));}
    assert(t.query(1,N)==(ll)N*(N+1)/2);
    for(int i=1;i<N;i+=2)assert(t.cut(i,i+1));
    assert(!t.connected(1,N));
}
''')

    def test_link_cut_noncommutative_path_composition(self):
        self.run_cpp('tree/link-cut.cpp', r'''
using Affine = pair<ll,ll>;
const ll MOD=998244353;
Affine compose(Affine f,Affine g){return {g.first*f.first%MOD,(g.first*f.second+g.second)%MOD};}
Affine affineIdentity(){return {1,0};}
int main(){
    mt19937 gen(7214);
    for(int trial=0;trial<40;++trial){
        int n=1+gen()%35;LinkCut<Affine,compose,affineIdentity> t(n);
        vector<vector<int>> g(n+1,vector<int>(n+1));vector<Affine> val(n+1,affineIdentity());
        auto path=[&](int u,int v){vector<int> par(n+1,-1),q{u};par[u]=0;
            for(int i=0;i<(int)q.size();++i)for(int y=1;y<=n;++y)
                if(g[q[i]][y]&&par[y]<0)par[y]=q[i],q.push_back(y);
            vector<int> ans;if(par[v]>=0)for(int x=v;x;x=par[x])ans.push_back(x);
            reverse(ans.begin(),ans.end());return ans;};
        for(int step=0;step<3000;++step){
            int u=1+gen()%n,v=1+gen()%n,op=gen()%5;auto p=path(u,v);
            if(op==0){assert(t.link(u,v)==p.empty());if(p.empty())g[u][v]=g[v][u]=1;}
            if(op==1){assert(t.cut(u,v)==bool(g[u][v]));g[u][v]=g[v][u]=0;}
            if(op==2){val[u]={gen()%MOD,gen()%MOD};t.set(u,val[u]);}
            if(op==3)t.makeRoot(u);
            if(op==4)assert(t.connected(u,v)==!p.empty());
            assert(t.query(u,u)==val[u]);
            p=path(u,v);if(!p.empty()){
                Affine f=affineIdentity();for(int x:p)f=compose(f,val[x]);assert(t.query(u,v)==f);
                reverse(p.begin(),p.end());f=affineIdentity();for(int x:p)f=compose(f,val[x]);
                assert(t.query(v,u)==f);
            }
        }
    }
}
''')

    def test_integration_analytic_integrals_and_limits(self):
        self.run_cpp('math/integrate.cpp', r'''
int main(){
    auto check=[](auto f,ld a,ld b,ld want,ld eps=1e-10L){
        auto [got,ok]=integrate(f,a,b,eps);assert(ok);assert(abs(got-want)<10*eps);
        auto [rev,rok]=integrate(f,b,a,eps);assert(rok&&abs(rev+want)<10*eps);
    };
    check([](ld x){return 2*x*x*x-3*x*x+4;},-2,3,35.L/2);
    check([](ld x){return exp(x);},-3,4,exp(4.L)-exp(-3.L));
    check([](ld x){return sin(x);},0,acos(-1.L),2);
    check([](ld x){return 1/(1+x*x);},-5,2,atan(2.L)+atan(5.L));
    auto [sq,sok]=integrate([](ld x){return sqrt(x);},0,1,1e-8L,60);
    assert(sok&&abs(sq-2.L/3)<1e-8L);
    check([](ld x){return exp(-x*x);},-6,6,sqrt(acos(-1.L)),1e-9L);
    int calls=0;auto f=[&](ld x){++calls;return exp(x);};
    auto [zero,zok]=integrate(f,1,1);assert(zero==0&&zok&&calls==0);
    for(int budget:{3,4,5,10,20}){calls=0;auto [v,ok]=integrate(f,0,1,1e-30L,25,budget);
        assert(!ok&&isfinite(v)&&calls<=budget);}
    calls=0;auto [v,ok]=integrate(f,0,1,1e-10L,0);assert(!ok&&calls==3&&isfinite(v));
}
''')


if __name__ == '__main__':
    unittest.main()
