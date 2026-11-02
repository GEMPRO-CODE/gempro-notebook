from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
COMPILER = shutil.which('g++') or shutil.which('clang++')


@unittest.skipUnless(COMPILER, 'C++ compiler required')
class ContestCoverageTests(unittest.TestCase):
    def run_cpp(self, paths, body, timeout=90):
        source = '#include <bits/stdc++.h>\n#include <cassert>\nusing namespace std;\n'
        for i, path in enumerate(paths):
            text = (ROOT / 'content' / path).read_text()
            # Test precisely the printed part, with its file-level declarations.
            source += text.split('// end template //')[0] + '\n'
        source += body
        with tempfile.TemporaryDirectory() as directory:
            p = Path(directory)
            (p / 'test.cpp').write_text(source)
            result = subprocess.run([COMPILER, '-std=c++20', '-O2', '-fsanitize=undefined',
                                     '-fno-sanitize-recover=all', str(p / 'test.cpp'),
                                     '-o', str(p / 'test')], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            result = subprocess.run([str(p / 'test')], capture_output=True, text=True, timeout=timeout)
            self.assertEqual(result.returncode, 0, result.stderr)

    def test_bit_basis_witnesses(self):
        self.run_cpp(['math/xor-basis.cpp'], r'''
int main(){mt19937 gen(712);
    for(int trial=0;trial<30;++trial){XorBasis<130>b;vector<bitset<130>>a;vi ids;
        for(int i=0;i<12;++i){bitset<130>x;unsigned v=gen()%256;
            for(int j=0;j<8;++j)x[j*17]=v>>j&1;
            auto residual=x;auto w=b.reduce(residual);bitset<130>sum;
            for(int j=0;j<130;++j)if(w[j]){assert(j<(int)ids.size());sum^=a[ids[j]];}
            assert((sum^residual)==x);
            bool exists=false;for(int mask=0;mask<(1<<i);++mask){bitset<130>z;
                for(int j=0;j<i;++j)if(mask>>j&1)z^=a[j];exists|=z==x;}
            assert(residual.none()==exists);a.push_back(x);
            assert(b.add(x)==exists);
            if(!exists)ids.push_back(i);assert(b.b.size()==ids.size());
        }
        for(int v=0;v<32;++v){bitset<130>x;
            for(int j=0;j<8;++j)x[j*17]=v>>j&1;
            auto original=x;auto w=b.reduce(x);bitset<130>sum;
            for(int j=0;j<130;++j)if(w[j])sum^=a[ids[j]];assert((sum^x)==original);
            bool exists=false;for(int mask=0;mask<(1<<12);++mask){bitset<130>z;
                for(int j=0;j<12;++j)if(mask>>j&1)z^=a[j];exists|=z==original;}
            assert(x.none()==exists);
        }
    }
    XorBasis<5>b;
    for(int i=0;i<1000;++i){bitset<5>x(16+(i%3)),r=x;auto w=b.reduce(r);
        if(r.none())assert((w.count()+1)%2==0);b.add(x);}
}
''')

    def test_unified_xor_basis_extrema(self):
        self.run_cpp(['math/xor-basis.cpp'], r'''
template<int B>void check(){using Basis=XorBasis<B>;using T=typename Basis::T;
    static_assert(is_same_v<T,bitset<B>>);
    static_assert(is_same_v<bool,decltype(declval<Basis&>().add(T{}))>);
    mt19937_64 gen(713+B);
    auto randomValue=[&](){T x;for(int i=0;i<B;++i)x[i]=gen()&1;return x;};
    auto less=[](const T&a,const T&b){for(int i=B-1;i>=0;--i)if(a[i]!=b[i])return b[i];return false;};
    for(int trial=0;trial<30;++trial){Basis basis;vector<T>a,span={T{}};vi ids;
        auto decode=[&](T w){T z;for(int j=0;j<B;++j)if(w[j]){
            assert(j<(int)ids.size());z^=a[ids[j]];}return z;};
        assert(basis.query()==T{}&&basis.query({},1)==T{});
        for(int id=0;id<8;++id){T x=id==0?T{}:randomValue();
            bool known=find(span.begin(),span.end(),x)!=span.end();
            T residual=x;auto w=basis.reduce(residual);assert((decode(w)^residual)==x);
            assert(residual.none()==known);a.push_back(x);assert(basis.add(x)==known);if(!known)ids.push_back(id);
            int size=span.size();for(int j=0;j<size;++j)span.push_back(span[j]^x);
            for(int q=0;q<12;++q){T v=randomValue(),lo=v,hi=v;
                for(auto y:span){T z=v^y;if(less(z,lo))lo=z;if(less(hi,z))hi=z;}
                assert(basis.query(v)==lo&&basis.query(v,1)==hi);
                T reduced=v;auto used=basis.reduce(reduced);assert(reduced==lo&&(v^decode(used))==lo);
                reduced=v;used=basis.reduce(reduced,true);assert(reduced==hi&&(v^decode(used))==hi);
            }
        }
    }
    // Fill every witness bit, then insert far more than B dependent values.
    Basis full;vector<T>original;vi ids;
    for(int i=B-1;i>=0;--i){original.push_back(T{});assert(full.add(T{}));
        T x;x[i]=1;original.push_back(x);assert(!full.add(x));ids.push_back(original.size()-1);}
    assert((int)ids.size()==B);
    for(int i=0;i<10000;++i){T x=randomValue(),r=x;auto w=full.reduce(r);T sum;
        for(int j=0;j<B;++j)if(w[j])sum^=original[ids[j]];
        assert(r.none()&&sum==x);assert(full.add(x));assert((int)full.b.size()==B);}
}
int main(){XorBasis plain;assert(!plain.add(5));assert(!plain.add(3));assert(plain.add(6));
    assert(plain.query(7).to_ullong()==1&&plain.query({},1).to_ullong()==6);
    XorBasis<64>full;assert(!full.add(UINT64_MAX));assert(full.query({},1).to_ullong()==UINT64_MAX);
    XorBasis<8>automatic;assert(automatic.add(0));assert(!automatic.add(5));assert(automatic.add(5));
    assert(!automatic.add(3));assert(automatic.add(6));assert(automatic.b.size()==2);
    bitset<8>target=6;auto w=automatic.reduce(target);assert(target.none()&&w==bitset<8>(3));
    XorBasis<1>late;for(int i=0;i<10000;++i)assert(late.add(0));
    assert(!late.add(1));bitset<1>one(1);assert(late.reduce(one)==bitset<1>(1)&&one.none());
    check<1>();check<5>();check<63>();check<64>();check<65>();check<130>();
}
''')

    def test_static_cht_envelope(self):
        self.run_cpp(['dsa/static-cht.cpp'], r'''
int main(){mt19937 gen(913);
    StaticCHT empty({});assert(empty.h.empty());
    for(int trial=0;trial<3000;++trial){vector<StaticCHT::Line>v;
        for(int i=0,n=1+gen()%60;i<n;++i)v.push_back({int(gen()%31)-15,int(gen()%201)-100});
        StaticCHT h(v);assert(isinf(h.start[0])&&h.start[0]<0);
        for(int i=1;i<(int)h.h.size();++i){assert(h.h[i-1].a<h.h[i].a&&h.start[i-1]<h.start[i]);
            assert(fabsl(h.h[i-1].at(h.start[i])-h.h[i].at(h.start[i]))<1e-10);}
        vector<ld>xs={-1e9L,0,1e9L};for(int i=1;i<(int)h.start.size();++i)
            for(ld d:{-1e-6L,0.L,1e-6L})xs.push_back(h.start[i]+d);
        for(int i=0;i<50;++i)xs.push_back((int(gen()%100001)-50000)/71.L);
        for(ld x:xs){ld want=-1e100L;for(auto l:v)want=max(want,l.at(x));
            assert(fabsl(h.query(x)-want)<1e-8L);}
    }
    StaticCHT h({{LLONG_MIN,LLONG_MAX},{LLONG_MAX,LLONG_MIN}});
    assert(h.query(0)==(ld)LLONG_MAX);
}
''')

    def test_kruskal_tree_minimax(self):
        self.run_cpp(['tree/kruskal-tree.cpp'], r'''
int main(){mt19937 gen(916);KruskalTree empty(0,{});assert(empty.roots.empty());
    for(int z=0;z<2500;++z){int n=1+gen()%15;vector<tuple<ll,int,int>>es;
        vector<vector<ll>>d(n,vector<ll>(n,LLONG_MAX));for(int i=0;i<n;++i)d[i][i]=LLONG_MIN;
        for(int k=0,m=gen()%80;k<m;++k){int u=gen()%n,v=gen()%n;ll w=int(gen()%41)-20;
            es.push_back({w,u,v});d[u][v]=d[v][u]=min(d[u][v],w);}
        for(int k=0;k<n;++k)for(int i=0;i<n;++i)for(int j=0;j<n;++j)d[i][j]=min(d[i][j],max(d[i][k],d[k][j]));
        KruskalTree t(n,es);int size=t.p.size();assert(size<=2*n-1);
        for(int i=n;i<size;++i){auto [a,b]=t.ch[i];assert(a<i&&b<i);assert(t.p[a]==i&&t.p[b]==i);
            assert(t.cnt[i]==t.cnt[a]+t.cnt[b]);}
        int total=0;for(int r:t.roots){assert(t.p[r]==-1);total+=t.cnt[r];}assert(total==n);
        for(int u=0;u<n;++u)for(int v=u+1;v<n;++v){set<int>anc;
            for(int x=u;x!=-1;x=t.p[x])anc.insert(x);
            int x=v;while(x!=-1&&!anc.count(x))x=t.p[x];
            if(d[u][v]==LLONG_MAX)assert(x==-1);else assert(x>=n&&t.w[x]==d[u][v]);}
    }
    // Force a long DSU chain before querying the oldest leaf.
    int n=200000;vector<tuple<ll,int,int>>edges;
    for(int i=1;i<n;++i)edges.push_back({i,i-1,i});
    edges.push_back({n,0,n-1});
    KruskalTree chain(n,edges);int root=2*n-2;
    assert(chain.roots==vi{root}&&chain.cnt[root]==n);
    assert(chain.w[root]==n-1&&chain.p[root]==-1);
    for(int i=n;i<=root;++i)assert(chain.cnt[i]==i-n+2);
}
''')

    def test_hash_shared_runtime_base(self):
        self.run_cpp(['strings/hashing.cpp', 'dsa/segtree.cpp'], r'''
void check(){
    using S=H;
    S a,b;
    assert(&a.base==&b.base && a.base>1 && a.base<H::M);
    assert(a.cnt==0 && a.h==0 && a==b);
    assert(S(0)!=S() && S(0).concat(S(0))!=S(0));
    mt19937_64 gen(213);
    for(int t=0;t<1000;++t){
        S x,y;ll want=0;int n=gen()%80,split=gen()%(n+1);
        for(int j=0;j<n;++j){ll v=ll(gen()%2001)-1000;
            if(j<split)x=x.concat(S(v));else y=y.concat(S(v));
            __int128 z=((__int128)want*S::base+v)%H::M;
            want=z<0?z+H::M:z;
        }
        auto oldx=x,oldy=y,z=x.concat(y);
        assert(z.h==want && z.cnt==n && x==oldx && y==oldy);
        assert(z.concat(S())==z && S().concat(z)==z);
        S c(17);assert(x.concat(y).concat(c)==x.concat(y.concat(c)));
        ll power=1;for(int i=0;i<n;++i)power=(__int128)power*S::base%S::M;
        assert(z.pw==power);
    }
}
using S=H;
S op(S a,S b){return a.concat(b);}
S e(){return {};}
int main(){
    check();
    for(ll a:{0LL,1LL,H::M-1}) for(ll b:{0LL,1LL,H::M-1}) {
        H x(a),y(b); auto z=x.concat(y);
        assert(z.h==((__int128)a*H::base+b)%H::M);
        assert(H::mul(a,b)==(__int128)a*b%H::M);
    }
    for(ll x:{LLONG_MIN,LLONG_MAX,-S::M,-1LL,0LL,S::M}){
        S a(x);assert(a.h==(x%S::M+S::M)%S::M && a.cnt==1);
        S b=a.concat(a);
        assert(b.h==((__int128)a.h*S::base+a.h)%S::M);
    }
    mt19937 gen(315);
    for(int n:{1,2,7,64,99}){
        Segtree<S,op,e> st(n);vector<ll>a(n);
        for(int i=0;i<n;++i)st.set(i,S(a[i]=gen()%256));
        for(int t=0;t<1000;++t){
            int i=gen()%n;st.set(i,S(a[i]=gen()%256));
            int l=gen()%(n+1),r=gen()%(n+1);if(l>r)swap(l,r);
            ll want=0;
            for(int j=l;j<r;++j)want=((__int128)want*S::base+a[j])%H::M;
            auto got=st.query(l,r);assert(got.h==want&&got.cnt==r-l);
        }
    }
    S zeros;
    for(int i=0;i<200000;++i)zeros=zeros.concat(S(0));
    S shifted=S(1).concat(zeros);ll want=1;
    for(int i=0;i<200000;++i)want=(__int128)want*S::base%S::M;
    assert(shifted.h==want&&shifted.cnt==200001&&shifted.pw==(__int128)want*S::base%S::M);
}
''')

    def test_matrix_semirings(self):
        self.run_cpp(['math/matrix.cpp'], r'''
int main(){mt19937 gen(192);for(int mod:{1,2,8,1000,1000000000}){
    auto add=[&](ll a,ll b){return (a+b)%mod;};auto mul=[&](ll a,ll b){return a*b%mod;};
    for(int z=0;z<100;++z){int n=1+gen()%5;Matrix<ll>a(n,vector<ll>(n));
        for(auto&r:a)for(auto&x:r)x=gen()%mod;Matrix<ll>r(n,vector<ll>(n));for(int i=0;i<n;++i)r[i][i]=1%mod;
        for(int e=0;e<15;++e){assert(matPow(a,e,0LL,ll(1%mod),add,mul)==r);
            Matrix<ll>next(n,vector<ll>(n));for(int i=0;i<n;++i)for(int j=0;j<n;++j)for(int k=0;k<n;++k)
                next[i][j]=(next[i][j]+r[i][k]*a[k][j])%mod;r=next;}
    }}
    using P=array<ll,4>;auto add=[](P a,P b){for(int i=0;i<4;++i)a[i]=(a[i]+b[i])%1000;return a;};
    auto mul=[](P a,P b){P c{};for(int i=0;i<4;++i)for(int j=0;j+i<4;++j)c[i+j]=(c[i+j]+a[i]*b[j])%1000;return c;};
    Matrix<P>a={{{1,1,0,0}}};auto r=matPow(a,10,P{},P{1,0,0,0},add,mul);
    assert((r[0][0]==P{1,10,45,120}));
    Matrix<int>x={{0,1,0},{0,0,1},{0,0,0}};
    auto b=matPow(x,2,0,1,[](int a,int b){return a|b;},[](int a,int b){return a&b;});
    assert(b[0][2]&&accumulate(b[0].begin(),b[0].end(),0)==1);
    Matrix<int>rect={{1,2,3},{4,5,6}},col={{7},{8},{9}};
    assert((matMul(rect,col,0,plus<int>(),multiplies<int>())==Matrix<int>{{50},{122}}));
}
''')

    def test_rollback_and_offline_wrapper(self):
        self.run_cpp(['dsa/rollback-segtree.cpp','dsa/offline-deletions.cpp'], r'''
struct Queries:RollbackChmax{
    vector<pair<int,int>>q;vector<ll>ans;
    Queries(int n):RollbackChmax(n,-100){}
    void handle(int t){auto [l,r]=q[t-1];ans.push_back(query(l,r));}
};
int main(){mt19937 gen(181);for(int trial=0;trial<100;++trial){int n=1+gen()%50;
    RollbackChmax s(n,LLONG_MIN);vector<vector<ll>>old;vector<ll>a(n,LLONG_MIN);
    for(int step=0;step<1000;++step){if(!old.empty()&&gen()%3==0){s.rollback();a=old.back();old.pop_back();}
        else{int l=gen()%(n+1),r=gen()%(n+1);if(l>r)swap(l,r);ll x=int(gen()%201)-100;
            if(step%31==0)x=LLONG_MAX;old.push_back(a);s.insert({l,r,x});for(int j=l;j<r;++j)a[j]=max(a[j],x);}
        for(int k=0;k<10;++k){int l=gen()%(n+1),r=gen()%(n+1);if(l>r)swap(l,r);
            ll want=LLONG_MAX;for(int j=l;j<r;++j)want=min(want,a[j]);assert(s.query(l,r)==want);}
    }}
    Queries ds(20);OfflineDeletion<Queries>off;set<Queries::Value>active;vector<ll>want;
    vector<Queries::Value>pool;for(int k=0;k<30;++k)pool.push_back({int(gen()%10),10+int(gen()%11),int(gen()%201)-100});
    for(int t=0;t<1000;++t){auto v=pool[gen()%pool.size()];if(gen()%2){off.insert(v);active.insert(v);}else{off.remove(v);active.erase(v);}
        vector<ll>a(20,-100);for(auto [l,r,x]:active)for(int j=l;j<r;++j)a[j]=max(a[j],x);
        int l=gen()%20,r=l+1+gen()%(20-l);ds.q.push_back({l,r});want.push_back(*min_element(a.begin()+l,a.begin()+r));}
    off.run(ds);assert(ds.ans==want);assert(ds.log.empty()&&ds.marks.empty());
}
''')

    def test_delaunay_exact_and_degenerate(self):
        self.run_cpp(['geometry/delaunay.cpp'], r"""
using P=Delaunay::Point;using I=__int128;
I turn(P a,P b,P c){return I(b.x-a.x)*(c.y-a.y)-I(b.y-a.y)*(c.x-a.x);}
I incircle(P a,P b,P c,P d){I x=a.x-d.x,y=a.y-d.y,u=b.x-d.x,v=b.y-d.y,s=c.x-d.x,t=c.y-d.y;
    return (x*x+y*y)*(u*t-v*s)-(u*u+v*v)*(x*t-y*s)+(s*s+t*t)*(x*v-y*u);}
void check(vector<P>p){Delaunay d(p);int n=p.size();set<pair<int,int>>edges;
    for(auto [u,v]:d.edges){assert(u>=0&&u<n&&v>=0&&v<n&&u!=v);if(u>v)swap(u,v);assert(edges.insert({u,v}).second);}
    vector<int>id(n);iota(id.begin(),id.end(),0);sort(id.begin(),id.end(),[&](int a,int b){return tie(p[a].x,p[a].y)<tie(p[b].x,p[b].y);});
    vector<int>h;for(int pass=0;pass<2;++pass){int k=h.size();for(int i:id){
        while((int)h.size()>=k+2&&turn(p[h[h.size()-2]],p[h.back()],p[i])<=0)h.pop_back();h.push_back(i);}
        if(!h.empty())h.pop_back();reverse(id.begin(),id.end());}
    I area=0;for(int i=0;i<(int)h.size();++i){P a=p[h[i]],b=p[h[(i+1)%h.size()]];area+=I(a.x)*b.y-I(a.y)*b.x;}
    I total=0;map<pair<int,int>,int>uses;
    for(auto t:d.triangles){auto [a,b,c]=t;assert(turn(p[a],p[b],p[c])>0);total+=turn(p[a],p[b],p[c]);
        for(int j=0;j<n;++j)assert(incircle(p[a],p[b],p[c],p[j])<=0);
        for(int j=0;j<3;++j){auto e=minmax(t[j],t[(j+1)%3]);assert(edges.count(e));assert(++uses[e]<=2);}}
    assert(area==total);
    if(!area){assert(d.triangles.empty());assert((int)edges.size()==max(0,n-1));}
    else assert((int)edges.size()==n+(int)d.triangles.size()-1);
    for(auto [a,b]:edges)for(auto [c,e]:edges){
        auto sign=[](I a){return (a>0)-(a<0);};
        assert(!(sign(turn(p[a],p[b],p[c]))*sign(turn(p[a],p[b],p[e]))<0&&
                 sign(turn(p[c],p[e],p[a]))*sign(turn(p[c],p[e],p[b]))<0));}
}
int main(){check({});check({{1,2}});check({{1,2},{3,4}});
    mt19937 gen(139);
    for(int n=2;n<80;++n){vector<P>p;for(int i=0;i<n;++i)p.push_back({3*i,7*i-100});shuffle(p.begin(),p.end(),gen);check(p);}
    vector<P>circle={{5,0},{4,3},{3,4},{0,5},{-3,4},{-4,3},{-5,0},{-4,-3},{-3,-4},{0,-5},{3,-4},{4,-3}};
    for(int z=0;z<30;++z){shuffle(circle.begin(),circle.end(),gen);check(circle);}
    for(int a=2;a<=9;++a){vector<P>p;for(int x=0;x<a;++x)for(int y=0;y<a;++y)p.push_back({x,y});check(p);}
    check({{-100000000,-100000000},{100000000,-100000000},{100000000,100000000},{-100000000,100000000},{0,0}});
    for(int z=0;z<1500;++z){set<pair<ll,ll>>points;int n=3+gen()%35;
        while((int)points.size()<n)points.insert({int(gen()%51)-25,int(gen()%51)-25});vector<P>p;
        for(auto [x,y]:points)p.push_back({x,y});shuffle(p.begin(),p.end(),gen);check(p);}
    vector<P>big;for(int i=0;i<100000;++i)big.push_back({i,int(gen()%10000000)});
    Delaunay d(big);assert(d.edges.size()<300000&&d.triangles.size()>100000);
}
""")

    def test_hull3d_support_and_manifold(self):
        self.run_cpp(['geometry/hull-3d.cpp'], r"""
void check(vector<Point3>p,bool full){auto f=hull3D(p);assert(f.empty()!=full);if(!full)return;
    map<pair<int,int>,int>edges;set<int>vertices;__int128 vol=0;
    for(auto t:f){auto [a,b,c]=t;vertices.insert(a);vertices.insert(b);vertices.insert(c);
        bool inside=false;for(auto q:p){auto v=volume6(p[a],p[b],p[c],q);assert(v<=0);inside|=v<0;}assert(inside);
        vol+=volume6({0,0,0},p[a],p[b],p[c]);
        for(int j=0;j<3;++j)assert(++edges[{t[j],t[(j+1)%3]}]==1);
    }
    for(auto [e,c]:edges)assert(edges.count({e.second,e.first}));
    assert(vol>0);assert(int(vertices.size())-int(edges.size()/2)+int(f.size())==2);
    // Independent supporting-triangle enumeration for general position inputs.
    bool general=true;int n=p.size();for(int a=0;a<n;++a)for(int b=a+1;b<n;++b)
        for(int c=b+1;c<n;++c)for(int d=c+1;d<n;++d)general &= volume6(p[a],p[b],p[c],p[d])!=0;
    if(general){set<array<int,3>>want,got;for(auto t:f){sort(t.begin(),t.end());got.insert(t);}
        for(int a=0;a<n;++a)for(int b=a+1;b<n;++b)for(int c=b+1;c<n;++c){bool pos=false,neg=false;
            for(auto q:p){auto v=volume6(p[a],p[b],p[c],q);pos|=v>0;neg|=v<0;}
            if(!(pos&&neg))want.insert({a,b,c});}assert(got==want);}
}
int main(){mt19937 gen(137);check({},false);check({{0,0,0},{1,0,0},{0,1,0}},false);
    check({{0,0,0},{0,0,0},{1,0,0},{0,1,0},{1,1,0}},false);
    check({{0,0,0},{0,0,0},{0,0,0},{1,0,0},{0,1,0},{0,0,1}},true);
    vector<Point3>cube;for(int x=0;x<2;++x)for(int y=0;y<2;++y)for(int z=0;z<2;++z)cube.push_back({x,y,z});
    for(int i=0;i<100;++i){shuffle(cube.begin(),cube.end(),gen);check(cube,true);}
    vector<Point3>grid;for(int x=0;x<3;++x)for(int y=0;y<3;++y)for(int z=0;z<3;++z)grid.push_back({x,y,z});
    for(int i=0;i<30;++i){shuffle(grid.begin(),grid.end(),gen);check(grid,true);}
    check({{-1000000000,-1000000000,-1000000000},{1000000000,-1000000000,-1000000000},
           {-1000000000,1000000000,-1000000000},{-1000000000,-1000000000,1000000000},{1000000000,1000000000,1000000000}},true);
    for(int i=0;i<1000;++i){vector<Point3>p={{0,0,0},{10,0,0},{0,10,0},{0,0,10}};
        for(int j=0,n=gen()%15;j<n;++j)p.push_back({int(gen()%41)-20,int(gen()%41)-20,int(gen()%41)-20});
        shuffle(p.begin(),p.end(),gen);check(p,true);}
}
""")

    def test_primality_nonpositive_and_small_inputs(self):
        self.run_cpp(['math/primality.cpp'], r'''
int main(){
    assert(!isPrime(LLONG_MIN));
    for(ll n=-100;n<=100000;++n){
        bool prime=n>=2;
        for(ll d=2;d*d<=n;++d)if(n%d==0){prime=false;break;}
        assert(isPrime(n)==prime);
    }
    assert(isPrime((1LL<<61)-1));
    assert(!isPrime(LLONG_MAX));
    assert(!isPrime(341550071728321LL));
}
''')

    def test_suffix_tree_structure_and_suffix_paths(self):
        self.run_cpp(['strings/suffix-tree.cpp'], r'''
void check(string s){
    int n=sz(s);vi sa(n),lcp(n);iota(all(sa),0);
    sort(all(sa),[&](int a,int b){return s.substr(a)<s.substr(b);});
    for(int i=1;i<n;++i)
        while(sa[i]+lcp[i]<n&&sa[i-1]+lcp[i]<n&&s[sa[i]+lcp[i]]==s[sa[i-1]+lcp[i]])++lcp[i];
    ST st(s,sa,lcp);vi seen(sz(st.t)),suffix(n);
    auto dfs=[&](auto self,int u,string path)->void{
        assert(!seen[u]++);assert(st.dep[u]==sz(path));
        auto &t=st.t[u];
        if(t.suf>=0){assert(t.suf<n);assert(path==s.substr(t.suf));++suffix[t.suf];}
        set<char> first;
        for(int v:t.ch){auto &c=st.t[v];assert(c.p==u);
            assert(0<=c.l&&c.l<=c.r&&c.r<=n);
            if(c.l<c.r)assert(first.insert(s[c.l]).second);
            self(self,v,path+s.substr(c.l,c.r-c.l));}
    };
    assert(st.t[0].p==-1);dfs(dfs,0,"");
    for(int x:seen)assert(x==1);
    for(int x:suffix)assert(x==1);
}
int main(){
    for(string s:{"","a","aaaaaa","banana$","banana","mississippi$","abababa"})check(s);
    mt19937 gen(2831);
    for(int t=0;t<2000;++t){string s;for(int i=0,n=gen()%40;i<n;++i)s+=char('a'+gen()%4);
        check(s);check(s+"$");}
}
''')

    def test_binary_search_wide_bounds(self):
        self.run_cpp(['dsa/binary-search.cpp'], r'''
int main(){
    for(ll target:{-4000000000000LL,0LL,4000000000000LL}){
        ll lo=-5000000000000LL,hi=5000000000000LL;
        binSearch(lo,hi,[&](ll x){return x>=target;});
        assert(hi==target&&lo==target-1);
    }
    ll lo=LLONG_MAX-100,hi=LLONG_MAX;
    binSearch(lo,hi,[](ll x){return x>=LLONG_MAX-7;});
    assert(hi==LLONG_MAX-7&&lo==hi-1);
}
''')

if __name__ == '__main__':
    unittest.main()
