import itertools
import json
from pathlib import Path
import random
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
COMPILER = shutil.which('g++') or shutil.which('clang++')


@unittest.skipUnless(COMPILER, 'C++ compiler required')
class ExpansionTests(unittest.TestCase):
    def run_cpp(self, name, body):
        source = '#include <cassert>\n#include <vector>\n#define main notebook_main\n#include '
        source += json.dumps(str(ROOT / 'content' / name)) + '\n#undef main\n' + body
        with tempfile.TemporaryDirectory() as directory:
            p = Path(directory)
            (p / 'test.cpp').write_text(source)
            result = subprocess.run([COMPILER, '-std=c++20', '-O2', str(p/'test.cpp'), '-o', str(p/'test')], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            result = subprocess.run([str(p/'test')], capture_output=True, text=True, timeout=45)
            self.assertEqual(result.returncode, 0, result.stderr)

    def test_gomory_hu_against_all_cuts(self):
        self.run_cpp('graph/gomory-hu.cpp', r'''
int main() {
    mt19937 gen(42);
    assert(gomoryHu(0,{}).empty()); assert(gomoryHu(1,{}).empty());
    for (int n=2;n<=8;++n) for(int trial=0;trial<100;++trial) {
        vector<tuple<int,int,ll>> es;
        for(int i=0;i<n*n;++i) if(gen()%3==0)
            es.emplace_back(gen()%n,gen()%n,gen()%9);
        auto tree=gomoryHu(n,es); assert(sz(tree)==n-1);
        vector<vector<pair<int,ll>>> adj(n);
        for(auto [u,v,w]:tree) adj[u].emplace_back(v,w),adj[v].emplace_back(u,w);
        for(int s=0;s<n;++s) for(int t=s+1;t<n;++t) {
            ll want=LLONG_MAX;
            for(int mask=0;mask<(1<<n);++mask) if((mask>>s&1)&&!(mask>>t&1)) {
                ll cut=0;
                for(auto [u,v,w]:es) if((mask>>u&1)!=(mask>>v&1)) cut+=w;
                want=min(want,cut);
            }
            vector<ll> dist(n,-1);dist[s]=LLONG_MAX;vi q={s};
            for(int i=0;i<sz(q);++i) for(auto [v,w]:adj[q[i]]) if(dist[v]<0)
                dist[v]=min(dist[q[i]],w),q.pb(v);
            assert(dist[t]==want);
        }
    }
}
''')

    def test_ntt_against_naive_and_changing_sizes(self):
        self.run_cpp('dsa/ntt.cpp', r'''
int main() {
    mt19937 gen(42);
    for(int t=0;t<500;++t) {
        int n=gen()%100,m=gen()%100;
        vector<ll>a(n),b(m),want(n&&m?n+m-1:0);
        for(ll &x:a)x=gen()%mod;for(ll &x:b)x=gen()%mod;
        for(int i=0;i<n;++i)for(int j=0;j<m;++j)want[i+j]=(want[i+j]+a[i]*b[j])%mod;
        assert(conv(a,b)==want);
    }
    for(int n:{1024,1,16384,2,256,65536,4}) {
        vector<ll>a(n,1),b(n,1);auto c=conv(a,b);
        for(int i=0;i<2*n-1;++i)assert(c[i]==min(i+1,2*n-1-i));
    }
}
''')

    def test_bostan_mori_against_direct_recurrence(self):
        self.run_cpp('math/bostan-mori.cpp', r'''
int main() {
    mt19937 gen(42);
    for(int trial=0;trial<300;++trial) {
        int d=1+gen()%40;vector<ll> s(d),c(d),seq(500);
        for(ll &x:s)x=gen()%mod;for(ll &x:c)x=gen()%mod;
        copy(all(s),seq.begin());
        for(int i=d;i<500;++i)for(int j=0;j<d;++j)seq[i]=(seq[i]+c[j]*seq[i-j-1])%mod;
        for(int k:{0,d-1,d,499}) assert(fastRec(s,c,k)==seq[k]);
        vector<ll> q(d+1),p(d);q[0]=1+gen()%(mod-1);
        for(int i=1;i<=d;++i)q[i]=gen()%mod;
        for(ll &x:p)x=gen()%mod;
        ll inv=powm(q[0],mod-2);
        for(int i=0;i<500;++i){ll x=i<d?p[i]:0;for(int j=1;j<=d&&j<=i;++j)x=(x-q[j]*seq[i-j]%mod+mod)%mod;seq[i]=x*inv%mod;}
        assert(bostanMori(p,q,499)==seq[499]);
    }
    assert(bostanMori({}, {1,2}, 99)==0);
    assert(fastRec({7},{3},1000000000000000000LL)==7*powm(3,1000000000000000000LL)%mod);
}
''')

    def test_formal_series_against_quadratic_definitions(self):
        self.run_cpp('math/power-series.cpp', r'''
int main() {
    mt19937 gen(42);
    for(int n=1;n<=100;++n) {
        vector<ll>a(n);for(ll &x:a)x=gen()%mod;a[0]=1+gen()%(mod-1);
        auto b=fpsInv(a,n);
        for(int i=0;i<n;++i){ll v=0;for(int j=0;j<=i;++j)v=(v+a[j]*b[i-j])%mod;assert(v==(i==0));}
        a[0]=0;auto e=fpsExp(a,n);vector<ll>w(n);w[0]=1;
        for(int i=1;i<n;++i){for(int j=1;j<=i;++j)w[i]=(w[i]+j*a[j]%mod*w[i-j])%mod;w[i]=w[i]*powm(i,mod-2)%mod;}
        assert(e==w);assert(fpsLog(e,n)==a);
        a[0]=1;assert(fpsExp(fpsLog(a,n),n)==a);
        vector<ll> one(n);one[0]=1;assert(fpsInv({1},n)==one);
    }
}
''')

    def test_discrete_log_against_orbit(self):
        self.run_cpp('math/discrete-log.cpp', r'''
int main() {
    for(ll m=1;m<=100;++m)for(ll a=0;a<=m+1;++a){
        vector<ll> first(m,-1);ll x=1%m,k=0;
        while(first[x]<0){first[x]=k++;x=x*a%m;}
        for(ll b=0;b<=m+1;++b)assert(discreteLog(a,b,m)==first[b%m]);
    }
    ll m=1000000007;assert(discreteLog(m-1,m-1,m)==1);
    assert(discreteLog(0,0,m)==1);assert(discreteLog(0,2,m)==-1);
}
''')

    def test_halfplanes_against_sequential_clipping(self):
        self.run_cpp('geometry/halfplane-intersection.cpp', r'''
#include <random>
ld area(vector<Pt> p){ld a=0;for(int i=0;i<sz(p);++i)a+=cross(p[i],p[(i+1)%sz(p)]);return abs(a)/2;}
vector<Pt> clip(vector<Pt> p,Halfplane h){
    vector<Pt> q;for(int i=0;i<sz(p);++i){Pt u=p[i],v=p[(i+1)%sz(p)];ld a=cross(h.d,u-h.p),b=cross(h.d,v-h.p);
    if(a>=0)q.pb(u);if((a<0)!=(b<0))q.pb(u+(v-u)*(a/(a-b)));}return q;
}
void check(vector<Halfplane> h,ld B=10){
    vector<Pt> ref={{-B,-B},{B,-B},{B,B},{-B,B}};
    for(auto a:h)ref=clip(ref,a);
    auto p=halfplaneInter(h,B);
    assert(abs(area(p)-area(ref))<1e-6L*(1+area(ref)));
    for(Pt v:p)for(auto a:h)assert(cross(a.d,v-a.p)>-1e-6L);
}
int main(){
    check({});check({{{0,0},{1,0}}});
    check({{{0,0},{1,0}},{{0,1},{1,1}},{{1,2},{0,2}}});
    check({{{0,1},{1,1}},{{1,0},{0,0}}}); // empty
    check({{{0,0},{1,0}},{{1,0},{0,0}}}); // line
    check({{{0,0},{1,0}},{{0,0},{0,-1}},{{0,0},{-1,1}}}); // point
    mt19937 gen(42);
    for(int t=0;t<3000;++t){
        vector<Halfplane> h;
        for(int i=0,n=gen()%40;i<n;++i){
            Pt p((int)(gen()%31)-15,(int)(gen()%31)-15),d;
            do{d=Pt((int)(gen()%9)-4,(int)(gen()%9)-4);}while(abs(d)==0);
            h.emplace_back(p,p+d);if(gen()%4==0)h.pb(h.back());
        }
        check(h);
    }
}
''')


def determinant(a):
    n = len(a)
    total = 0
    for p in itertools.permutations(range(n)):
        term = (-1) ** sum(p[i] > p[j] for i in range(n) for j in range(i+1,n))
        for i in range(n):
            term *= a[i][p[i]]
        total += term
    return total


class CountingReferenceTests(unittest.TestCase):
    def test_binary_cut_construction_for_every_assignment(self):
        rng = random.Random(42)
        for _ in range(500):
            n = 5
            unary = [[rng.randrange(-10,11) for _ in range(2)] for _ in range(n)]
            pairs = []
            for u in range(n):
                for v in range(u+1,n):
                    e00,e01,e10 = [rng.randrange(-10,11) for _ in range(3)]
                    e11 = e01+e10-e00-rng.randrange(10)
                    pairs.append((u,v,[e00,e01,e10,e11]))
            a = [d[1]-d[0] for d in unary]
            constant = sum(d[0] for d in unary)
            edges = []
            for u,v,(e00,e01,e10,e11) in pairs:
                constant += e00
                a[u] += e10-e00
                a[v] += e11-e10
                edges.append((u,v,e01+e10-e00-e11))
            constant += sum(min(x,0) for x in a)
            for bits in itertools.product(range(2),repeat=n):
                want = sum(unary[i][bits[i]] for i in range(n))
                want += sum(e[2*bits[u]+bits[v]] for u,v,e in pairs)
                got = constant+sum(w for u,v,w in edges if bits[u]==0 and bits[v]==1)
                got += sum(max(x,0)*bits[i]+max(-x,0)*(1-bits[i]) for i,x in enumerate(a))
                self.assertEqual(got,want)

    def test_directed_kirchhoff_and_best_against_enumeration(self):
        rng = random.Random(42)
        for _ in range(200):
            n = rng.randrange(1,5)
            # Union of closed walks through 0: connected Eulerian, with loops/parallel arcs.
            edges = []
            for unused in range(rng.randrange(1,3)):
                walk = [0]+[rng.randrange(n) for _ in range(rng.randrange(1,4))]+[0]
                edges.extend(zip(walk,walk[1:]))
            active = sorted(set(sum(([u,v] for u,v in edges),[])))
            lap = [[0]*n for _ in range(n)]
            for u,v in edges:
                if u != v:
                    lap[u][u] += 1; lap[u][v] -= 1
            nonroot = [v for v in active if v != 0]
            trees = determinant([[lap[u][v] for v in nonroot] for u in nonroot])
            brute = 0
            choices = [[v for u,v in edges if u==x] for x in nonroot]
            for dests in itertools.product(*choices):
                parent = dict(zip(nonroot,dests))
                valid = True
                for v in nonroot:
                    seen = set()
                    while v != 0 and v not in seen:
                        seen.add(v);v = parent[v]
                    valid &= v==0
                brute += valid
            self.assertEqual(trees,brute)
            import math
            expected = trees
            for v in active:
                expected *= math.factorial(sum(u==v for u,w in edges)-1)
            def tours(v,mask):
                if mask == (1<<len(edges))-1:
                    return int(v==0)
                return sum(tours(w,mask|(1<<i)) for i,(u,w) in enumerate(edges) if u==v and not mask>>i&1)
            self.assertEqual(expected,tours(edges[0][1],1))

    def test_lgv_signed_count_against_disjoint_paths(self):
        rng = random.Random(42)
        saw_negative = False
        for _ in range(200):
            n = 7
            adj = [[v for v in range(u+1,n) if rng.randrange(3)==0] for u in range(n)]
            def paths(u,v):
                if u==v: return [frozenset([u])]
                return [p|{u} for w in adj[u] for p in paths(w,v)]
            sources,targets = [0,1],[5,6]
            p = [[paths(u,v) for v in targets] for u in sources]
            count = lambda a,b: sum(not x&y for x in a for y in b)
            signed = count(p[0][0],p[1][1])-count(p[0][1],p[1][0])
            self.assertEqual(determinant([[len(x) for x in row] for row in p]),signed)
            saw_negative |= signed<0
        self.assertTrue(saw_negative)


if __name__ == '__main__':
    unittest.main()
