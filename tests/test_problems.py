from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
COMPILER = shutil.which('g++') or shutil.which('clang++')


@unittest.skipUnless(COMPILER, 'C++ compiler required')
class ProblemsTests(unittest.TestCase):
    def run_cpp(self, name, body):
        source = ('#include <bits/stdc++.h>\n#include <cassert>\n'
                  f'#include "{name}.cpp"\n' + body)
        with tempfile.TemporaryDirectory() as directory:
            p = Path(directory)
            (p / 'test.cpp').write_text(source)
            result = subprocess.run(
                [COMPILER, '-std=c++20', '-O2', '-fsanitize=undefined',
                 '-fno-sanitize-recover=all', '-I', str(ROOT / 'content/problems'),
                 str(p / 'test.cpp'), '-o', str(p / 'test')],
                capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            result = subprocess.run([str(p / 'test')], capture_output=True,
                                    text=True, timeout=90)
            self.assertEqual(result.returncode, 0, result.stderr)

    def test_reroot(self):
        self.run_cpp('reroot', r'''
int main() {
    mt19937 gen(101);
    using T = pair<long long,long long>;
    auto merge = [](T a,T b) { return T{a.first+b.first,a.second+b.second}; };
    auto vertex = [](T a,int) { return T{a.first+1,a.second}; };
    for (int trial=0; trial<500; ++trial) {
        int n=1+gen()%35;
        vector<vi> g(n); vector<vi> w(n,vi(n));
        for (int i=1; i<n; ++i) {
            int p=gen()%i; g[i].push_back(p); g[p].push_back(i);
            w[i][p]=w[p][i]=int(gen()%11)-5;
        }
        for (auto &a:g) shuffle(a.begin(),a.end(),gen);
        auto edge = [&](T a,int u,int v) { return T{a.first,a.second+a.first*w[u][v]}; };
        auto ans=reroot(g,T{},merge,vertex,edge);
        for (int root=0; root<n; ++root) {
            long long sum=0;
            auto dfs = [&](auto &&self,int u,int p,long long d)->void {
                sum+=d;
                for (int v:g[u]) if (v!=p) self(self,v,u,d+w[u][v]);
            };
            dfs(dfs,root,-1,0);
            assert(ans[root]==T(n,sum));
        }
        // Order-sensitive aggregation catches misplaced parent contributions.
        auto cat=[](string a,string b){return a+b;};
        auto node=[](string a,int u){return "["+to_string(u)+":"+a+"]";};
        auto lift=[](string a,int u,int v){return to_string(u)+">"+to_string(v)+a;};
        auto ordered=reroot(g,string{},cat,node,lift);
        auto dfs=[&](auto &&self,int u,int p)->string {
            string a;
            for (int v:g[u]) if (v!=p) a+=lift(self(self,v,u),v,u);
            return node(a,u);
        };
        for (int r=0;r<n;++r) assert(ordered[r]==dfs(dfs,r,-1));
    }
    int n=200000; vector<vi> chain(n);
    for(int i=1;i<n;++i) chain[i].push_back(i-1),chain[i-1].push_back(i);
    auto edge=[](T a,int,int){return T{a.first,a.second+a.first};};
    auto ans=reroot(chain,T{},merge,vertex,edge);
    for(long long i=0;i<n;++i)
        assert(ans[i]==T(n,i*(i+1)/2+(n-1-i)*(n-i)/2));
}
''')

    def test_potential_dsu(self):
        self.run_cpp('potential-dsu', r'''
template<bool parity> void check() {
    mt19937 gen(102+parity);
    for(int trial=0;trial<200;++trial) {
        int n=1+gen()%25;
        PotentialDSU<parity> ds(n);
        vector<vector<pair<int,ll>>> g(n);
        auto sum=[](ll a,ll b){return parity ? a^b : a+b;};
        auto neg=[](ll a){return parity ? a : -a;};
        auto distances=[&](int u) {
            vector<optional<ll>> dist(n); dist[u]=0;
            vector<int> q{u};
            for(int i=0;i<int(q.size());++i)
                for(auto [v,w]:g[q[i]]) if(!dist[v]) {
                    dist[v]=sum(*dist[q[i]],w); q.push_back(v);
                }
            return dist;
        };
        for(int step=0;step<100;++step) {
            int u=gen()%n,v=gen()%n;
            ll w=parity ? gen()%2 : int(gen()%31)-15;
            auto dist=distances(u);
            bool ok=!dist[v] || *dist[v]==w;
            assert(ds.merge(u,v,w)==ok);
            if(ok) g[u].emplace_back(v,w),g[v].emplace_back(u,neg(w));
            dist=distances(u);
            for(int i=0;i<n;++i) assert(ds.diff(u,i)==dist[i]);
            // A successful union may have connected previously separate components.
            auto after=distances(v);
            for(int i=0;i<n;++i) assert(ds.diff(v,i)==after[i]);
        }
    }
}
int main(){check<false>();check<true>();}
''')

    def test_rectangle_union(self):
        self.run_cpp('rectangle-union', r'''
int main() {
    mt19937 gen(104);
    for(int trial=0;trial<2500;++trial) {
        vector<array<ll,4>> rs;
        int n=gen()%15;
        for(int i=0;i<n;++i) {
            ll a=int(gen()%11)-5,b=int(gen()%11)-5;
            ll c=int(gen()%11)-5,d=int(gen()%11)-5;
            if(a>c)swap(a,c); if(b>d)swap(b,d);
            rs.push_back({a,b,c,d});
            if(gen()%4==0)rs.push_back(rs.back());
        }
        int expected=0;
        for(int x=-5;x<5;++x) for(int y=-5;y<5;++y) {
            bool covered=false;
            for(auto [a,b,c,d]:rs) covered|=a<=x&&x<c&&b<=y&&y<d;
            expected+=covered;
        }
        assert(rectUnion(rs)==expected);
    }
    ll B=1000000000000000000LL;
    assert(rectUnion({{-B,-B,B,B},{-B,-B,B,B}})==i128(2*B)*(2*B));
    assert(rectUnion({{0,0,1,1},{1,0,2,1},{0,1,2,2}})==4);
}
''')

    def test_bounded_knapsack(self):
        self.run_cpp('bounded-knapsack', r'''
int main() {
    mt19937 gen(105);
    for(int trial=0;trial<2000;++trial) {
        int W=gen()%40,n=gen()%12;
        vector<tuple<int,ll,int>> items;
        vector<ll> dp(W+1,LLONG_MIN); dp[0]=0;
        for(int i=0;i<n;++i) {
            int w=1+gen()%50,c=gen()%6; ll v=int(gen()%31)-15;
            if(gen()%20==0)c=INT_MAX;
            items.emplace_back(w,v,c);
            auto old=dp;
            for(int s=0;s<=W;++s) if(old[s]!=LLONG_MIN)
                for(int k=1;k<=min(c,(W-s)/w);++k)
                    dp[s+k*w]=max(dp[s+k*w],old[s]+k*v);
        }
        assert(boundedKnapsack(W,items)==dp);
    }
    assert(boundedKnapsack(3,{{1,-5,2}})==vector<ll>({0,-5,-10,LLONG_MIN}));
}
''')

    def test_fenwick_2d(self):
        self.run_cpp('fenwick-2d', r'''
int main() {
    mt19937 gen(106);
    for(int trial=0;trial<200;++trial) {
        vector<pair<ll,ll>> pts;
        int n=gen()%35;
        for(int i=0;i<n;++i) pts.emplace_back(int(gen()%21)-10,int(gen()%21)-10);
        Fenwick2D f(pts);
        vector<tuple<ll,ll,ll>> updates;
        for(int step=0;step<200;++step) {
            if(n && gen()%2) {
                auto [x,y]=pts[gen()%n]; ll v=int(gen()%21)-10;
                f.add(x,y,v); updates.emplace_back(x,y,v);
            }
            ll a=int(gen()%31)-15,b=int(gen()%31)-15;
            ll c=int(gen()%31)-15,d=int(gen()%31)-15;
            if(a>c)swap(a,c);if(b>d)swap(b,d);
            ll pref=0,rect=0;
            for(auto [x,y,v]:updates) {
                if(x<c&&y<d)pref+=v;
                if(a<=x&&x<c&&b<=y&&y<d)rect+=v;
            }
            assert(f.sum(c,d)==pref && f.rect(a,b,c,d)==rect);
        }
    }
    Fenwick2D f({{LLONG_MIN,LLONG_MIN},{LLONG_MAX,LLONG_MAX}});
    f.add(LLONG_MIN,LLONG_MIN,7);f.add(LLONG_MAX,LLONG_MAX,8);
    assert(f.sum(LLONG_MAX,LLONG_MAX)==7);
    assert(f.sum(LLONG_MIN,LLONG_MIN)==0);
}
''')

    def test_interval_assignment(self):
        self.run_cpp('interval-assignment', r'''
int main() {
    mt19937 gen(107);
    for(int trial=0;trial<200;++trial) {
        int n=1+gen()%60; ll initial=int(gen()%11)-5;
        Intervals f(n,initial); vector<ll> a(n,initial);
        for(int step=0;step<500;++step) {
            int l=gen()%(n+1),r=gen()%(n+1);if(l>r)swap(l,r);
            ll v=int(gen()%11)-5;
            f.assign(l,r,v);fill(a.begin()+l,a.begin()+r,v);
            for(int i=0;i<n;++i)assert(f.get(i)==a[i]);
            assert(f.m.begin()->first==0&&f.m.rbegin()->first==n);
            int runs=1;
            for(int i=1;i<n;++i)runs+=a[i]!=a[i-1];
            assert(int(f.m.size())==runs+1);
            for(auto it=f.m.begin();next(it)!=f.m.end();++it) {
                auto jt=next(it);
                if(jt->first<n)assert(it->second!=jt->second);
                for(int i=it->first;i<jt->first;++i)assert(a[i]==it->second);
            }
        }
        f.assign(0,n,42);assert(f.m.size()==2);
    }
    Intervals f(LLONG_MAX,-3);
    f.assign(0,LLONG_MAX,8);f.assign(LLONG_MAX-1,LLONG_MAX,9);
    assert(f.get(0)==8&&f.get(LLONG_MAX-1)==9&&f.m.size()==3);
}
''')

    def test_bounded_flow(self):
        self.run_cpp('bounded-flow', r'''
int main() {
    mt19937 gen(108);
    for(int trial=0;trial<2000;++trial) {
        int n=1+gen()%5,m=gen()%8;
        vector<array<int,4>> es;
        BoundedFlow f(n);
        for(int i=0;i<m;++i) {
            int u=gen()%n,v=gen()%n,l=gen()%3,r=l+gen()%3;
            es.push_back({u,v,l,r});assert(f.add(u,v,l,r)==i);
        }
        vector<int> bal(n);
        auto brute=[&](auto &&self,int i)->bool {
            if(i==m)return all_of(bal.begin(),bal.end(),[](int x){return x==0;});
            auto [u,v,l,r]=es[i];
            for(int x=l;x<=r;++x) {
                bal[u]-=x;bal[v]+=x;
                bool ok=self(self,i+1);
                bal[u]+=x;bal[v]-=x;
                if(ok)return true;
            }
            return false;
        };
        bool expected=brute(brute,0);
        assert(f.feasible()==expected);
        if(expected) {
            vector<ll> net(n);
            for(int i=0;i<m;++i) {
                auto [u,v,l,r]=es[i];ll x=f.get(i);
                assert(l<=x&&x<=r);net[u]-=x;net[v]+=x;
            }
            for(ll x:net)assert(x==0);
        }
    }
    for(int k=0;k<=7;++k) {
        BoundedFlow f(3);
        int a=f.add(0,1,2,5),b=f.add(1,2,1,4),back=f.add(2,0,k,k);
        bool ok=f.feasible();assert(ok==(2<=k&&k<=4));
        if(ok)assert(f.get(a)==k&&f.get(b)==k&&f.get(back)==k);
    }
    BoundedFlow range(2);range.add(0,1,3,7);
    int back=range.add(1,0,4,5);assert(range.feasible());
    assert(4<=range.get(back)&&range.get(back)<=5);
}
''')


if __name__ == '__main__':
    unittest.main()
