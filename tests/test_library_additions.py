from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
COMPILER = shutil.which('g++') or shutil.which('clang++')


@unittest.skipUnless(COMPILER, 'C++ compiler required')
class LibraryAdditionsTests(unittest.TestCase):
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

    def test_monoid_deque(self):
        self.run_cpp('dsa/monoid-deque', r'''
using S=pair<long long,long long>;
const long long mod=998244353;
long long calls=0;
S compose(S a,S b){return {a.first*b.first%mod,(a.second*b.first+b.second)%mod};}
S op(S a,S b){++calls;return compose(a,b);}
S e(){return {1,0};}
int main(){
    mt19937 gen(711);
    MonoidDeque<S,op,e> d;deque<S> a;
    for(int i=0;i<30000;++i){
        int type=gen()%4;if(a.empty())type%=2;
        S x{gen()%mod,gen()%mod};
        if(type==0)d.push_front(x),a.push_front(x);
        if(type==1)d.push_back(x),a.push_back(x);
        if(type==2)d.pop_front(),a.pop_front();
        if(type==3)d.pop_back(),a.pop_back();
        S expected=e();for(auto v:a)expected=compose(expected,v);
        assert(d.size()==int(a.size())&&d.query()==expected);
        if(!a.empty())assert(d.front()==a.front()&&d.back()==a.back());
    }
    while(d.size())d.pop_front();
    assert(d.query()==e());
    calls=0;
    for(int round=0;round<4;++round){
        for(int i=0;i<20000;++i)
            if(round&1)d.push_front({2,i});else d.push_back({2,i});
        for(int i=0;i<20000;++i)
            if(i&1)d.pop_front();else d.pop_back();
    }
    assert(calls<20*160000); // Rebalance by halves, not the whole opposite stack.
    for(int i=0;i<10000;++i){
        d.push_front({0,i});assert(d.back()==S(0,i));d.pop_back();
        d.push_back({1,i});assert(d.front()==S(1,i));d.pop_front();
    }
}
''')

    def test_xor_trie(self):
        self.run_cpp('dsa/xor-trie', r'''
template<int B>void check(){
    using T=uint64_t;mt19937_64 gen(712+B);
    XorTrie<B> t;vector<T>a;
    T mask=UINT64_MAX>>(64-B);
    for(int step=0;step<8000;++step){
        T x=gen()&mask;
        if(step%17==0)x=mask;
        if(a.empty()||((gen()%2)&&a.size()<70)){
            if(!a.empty()&&gen()%3==0)x=a[gen()%a.size()];
            a.push_back(x);t.add(x);
        }else{int i=gen()%a.size();t.add(a[i],-1);a.erase(a.begin()+i);}
        assert(t.size()==int(a.size()));
        assert(t.count(x)==count(a.begin(),a.end(),x));
        vector<T>b;for(T v:a)b.push_back(v^x);sort(b.begin(),b.end());
        for(int k=0;k<int(b.size());++k)assert(t.kth(k,x)==b[k]);
        b=a;sort(b.begin(),b.end());
        for(int k=0;k<int(b.size());++k)assert(t.kth(k)==b[k]);
    }
    for(T x:a)t.add(x,-1);
    assert(t.size()==0);
    size_t before=t.t.size();t.add(mask);t.add(mask,-1);
    size_t after=t.t.size();assert(after<=before+B);
    t.add(mask);assert(t.t.size()==after&&t.kth(0)==mask);
}
int main(){check<1>();check<8>();check<30>();check<64>();}
''')

    def test_tree_isomorphism(self):
        self.run_cpp('tree/isomorphism', r'''
vector<string> shapes(vector<vi>&g,int root){
    vector<string>a(g.size());
    auto dfs=[&](auto&&self,int u,int p)->void{
        vector<string>ch;
        for(int v:g[u])if(v!=p){self(self,v,u);ch.push_back(a[v]);}
        sort(ch.begin(),ch.end());a[u]="(";
        for(auto&s:ch)a[u]+=s;a[u]+=")";
    };dfs(dfs,root,-1);return a;
}
int main(){mt19937 gen(713);TreeIso iso;
    map<string,int> known;map<int,string> inverse;
    for(int trial=0;trial<500;++trial){
        int n=1+gen()%50,r=gen()%n;vector<vi>g(n);
        for(int i=1;i<n;++i){int p=gen()%i;g[p].push_back(i);g[i].push_back(p);}
        auto ids=iso.classify(g,r);auto s=shapes(g,r);
        for(int i=0;i<n;++i){
            auto [it,ok]=known.emplace(s[i],ids[i]);assert(it->second==ids[i]);
            auto [jt,ok2]=inverse.emplace(ids[i],s[i]);assert(jt->second==s[i]);
        }
        vi perm(n);iota(perm.begin(),perm.end(),0);shuffle(perm.begin(),perm.end(),gen);
        vector<vi>h(n);
        for(int u=0;u<n;++u)for(int v:g[u])h[perm[u]].push_back(perm[v]);
        for(auto &v:h)shuffle(v.begin(),v.end(),gen);
        auto other=iso.classify(h,perm[r]);
        for(int i=0;i<n;++i)assert(ids[i]==other[perm[i]]);
    }
    int n=100000;vector<vi>chain(n);
    for(int i=1;i<n;++i)chain[i].push_back(i-1),chain[i-1].push_back(i);
    TreeIso fresh;auto ids=fresh.classify(chain);
    assert(set<int>(ids.begin(),ids.end()).size()==size_t(n));
}
''')

    def test_complement_components(self):
        self.run_cpp('graph/complement-components', r'''
void check(vector<vi>&g){
    int n=g.size();vector<vector<bool>>adj(n,vector<bool>(n));
    for(int u=0;u<n;++u)for(int v:g[u])adj[u][v]=true;
    vi want(n,-1);int k=0;
    for(int s=0;s<n;++s)if(want[s]<0){
        vi q{s};want[s]=k++;
        for(int i=0;i<int(q.size());++i)for(int v=0;v<n;++v)
            if(want[v]<0&&!adj[q[i]][v])want[v]=want[s],q.push_back(v);
    }
    auto comps=complementCC(g);assert(int(comps.size())==k);
    vi got(n,-1);
    for(int c=0;c<int(comps.size());++c)for(int u:comps[c]){
        assert(0<=u&&u<n&&got[u]<0);got[u]=c;
    }
    for(int i=0;i<n;++i){assert(got[i]>=0);
        for(int j=0;j<n;++j)assert((got[i]==got[j])==(want[i]==want[j]));}
}
int main(){
    for(int n=0;n<=6;++n)for(int mask=0;mask<(1<<(n*(n-1)/2));++mask){
        vector<vi>g(n);int k=0;
        for(int u=0;u<n;++u)for(int v=u+1;v<n;++v,++k)if(mask>>k&1)
            g[u].push_back(v),g[v].push_back(u);
        check(g);
    }
    mt19937 gen(714);
    for(int it=0;it<200;++it){int n=1+gen()%60;vector<vi>g(n);
        for(int u=0;u<n;++u)for(int v=u+1;v<n;++v)if(gen()%3)
            g[u].push_back(v),g[v].push_back(u);
        for(auto&v:g)shuffle(v.begin(),v.end(),gen);check(g);
    }
    vector<vi>empty(200000);auto one=complementCC(empty);
    assert(one.size()==1&&one[0].size()==empty.size());
    vector<vi>star(100000);
    for(int i=1;i<int(star.size());++i)star[0].push_back(i),star[i].push_back(0);
    auto two=complementCC(star);assert(two.size()==2&&two[0].size()==1);
}
''')


if __name__ == '__main__':
    unittest.main()
