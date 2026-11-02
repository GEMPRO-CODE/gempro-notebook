from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
COMPILER = shutil.which('g++') or shutil.which('clang++')


@unittest.skipUnless(COMPILER, 'C++ compiler required')
class SegtreeBeatsTests(unittest.TestCase):
    def run_cpp(self, body):
        source = ('#include <bits/stdc++.h>\n#include <cassert>\n'
                  '#include "segtree-beats.cpp"\n' + body)
        with tempfile.TemporaryDirectory() as directory:
            p = Path(directory)
            (p / 'test.cpp').write_text(source)
            result = subprocess.run(
                [COMPILER, '-std=c++20', '-O2', '-fsanitize=undefined',
                 '-fno-sanitize-recover=all', '-I', str(ROOT / 'content/dsa'),
                 str(p / 'test.cpp'), '-o', str(p / 'test')],
                capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            result = subprocess.run([str(p / 'test')], capture_output=True,
                                    text=True, timeout=90)
            self.assertEqual(result.returncode, 0, result.stderr)

    def test_chmin_sum(self):
        self.run_cpp(r'''
using namespace chmin_sum;
void check(Tree &t,const vector<ll> &a,int l,int r) {
    S expected=e();
    for(int i=l;i<r;++i)expected=op(expected,leaf(a[i]));
    auto got=t.query(l,r);
    assert(got.sum==expected.sum&&got.mx==expected.mx);
    assert(got.second==expected.second&&got.count==expected.count);
}
int main() {
    mt19937 gen(902);
    for(int trial=0;trial<250;++trial) {
        int n=1+gen()%70;
        Tree t(n);vector<ll>a(n);
        assert(t.t.size()==1);
        for(int step=0;step<400;++step) {
            int l=gen()%(n+1),r=gen()%(n+1);if(l>r)swap(l,r);
            ll x=int(gen()%101)-50;
            switch(gen()%3) {
            case 0:
                t.apply(l,r,x);
                for(int i=l;i<r;++i)a[i]=min(a[i],x);
                break;
            case 1: {
                int p=gen()%n;t.set(p,leaf(x));a[p]=x;break;
            }
            case 2:check(t,a,l,r);break;
            }
            check(t,a,0,n);
        }
        for(int l=0;l<=n;++l)for(int r=l;r<=n;++r)check(t,a,l,r);
    }
    // Equality with the second maximum must force descent.
    Tree t(513);vector<ll>a(513);
    for(int i=0;i<513;++i)t.set(i,leaf(a[i]=i));
    for(int cap=512;cap>=-2;--cap) {
        t.apply(0,513,cap);
        for(auto &x:a)x=min(x,ll(cap));
        check(t,a,0,513);check(t,a,17,499);
    }
}
''')

    def test_sparse_domain(self):
        self.run_cpp(r'''
using namespace chmin_sum;
int main() {
    ll n=(1LL<<60)+17;
    Tree uniform(n);
    uniform.apply(0,n,-2);
    assert(uniform.t.size()==1&&uniform.query(0,n).sum==-2*n);
    uniform.apply(n/2,n/2,-100);
    assert(uniform.query(n/2,n/2).count==0&&uniform.t.size()==1);
    uniform.set(n-1,leaf(17));
    assert(uniform.query(0,n).sum==-2*(n-1)+17);
    assert(uniform.query(n-1,n).sum==17);
    assert(uniform.query(0,1).sum==-2);
    uniform.apply(0,n,-3);
    assert(uniform.query(0,n).sum==-3*n);
    assert(uniform.t.size()<300);

    n=LLONG_MAX;
    Tree t(n);map<ll,ll>a;mt19937_64 gen(903);
    size_t budget=1;
    for(int step=0;step<1500;++step) {
        // At most two children per level for each operation boundary.
        budget+=2*63*(step%3==0 ? 1 : 2);
        ll l=gen()%n,r=gen()%n;if(l>r)swap(l,r);
        if(step%3==0) {
            ll p=step==0?n-1:gen()%n,x=int(gen()%101)-50;
            t.set(p,leaf(x));a[p]=x;
        } else if(step%3==1) {
            ll cap=gen()%51;t.apply(l,r,cap);
            for(auto it=a.lower_bound(l);it!=a.end()&&it->first<r;++it)
                it->second=min(it->second,cap);
        } else {
            ll expected=0;
            for(auto it=a.lower_bound(l);it!=a.end()&&it->first<r;++it)
                expected+=it->second;
            assert(t.query(l,r).sum==expected);
        }
    }
    ll expected=0;for(auto [p,x]:a)expected+=x;
    assert(t.query(0,n).sum==expected);
    assert(t.t.size()<=budget);
}
''')

    def test_affine_composition(self):
        self.run_cpp(r'''
namespace affine {
const ll M=998244353;
struct S {ll sum,len;};
struct U {ll a,b;};
S e(){return {0,0};}
S init(ll l,ll r){return {((l+r-1)*(r-l)/2)%M,r-l};}
S op(S a,S b){return {(a.sum+b.sum)%M,a.len+b.len};}
bool mapping(U u,S &s){s.sum=(u.a*s.sum+u.b*s.len)%M;return true;}
U compose(U f,U g){return {f.a*g.a%M,(f.a*g.b+f.b)%M};}
U id(){return {1,0};}
using Tree=SegtreeBeats<S,op,e,U,mapping,compose,id,init>;
}
int main(){using namespace affine;mt19937 gen(904);
    for(int trial=0;trial<100;++trial){
        int n=1+gen()%80;Tree t(n);vector<ll>a(n);iota(a.begin(),a.end(),0);
        for(int step=0;step<400;++step){
            int l=gen()%(n+1),r=gen()%(n+1);if(l>r)swap(l,r);
            if(step%5==0)l=0,r=n;
            U f{gen()%7,gen()%17};
            if(step%3==0){int p=gen()%n;ll x=gen()%100;t.set(p,{x,1});a[p]=x;}
            else {t.apply(l,r,f);for(int i=l;i<r;++i)a[i]=(f.a*a[i]+f.b)%M;}
            ll expected=0;for(int i=l;i<r;++i)expected=(expected+a[i])%M;
            auto got=t.query(l,r);assert(got.sum==expected&&got.len==r-l);
        }
    }
}
''')

    def test_push_failure_and_order(self):
        self.run_cpp(r'''
namespace strings {
string original;
string e(){return {};}
string init(ll l,ll r){return original.substr(l,r-l);}
string op(string a,string b){return a+b;}
bool mapping(char u,string &s){
    if(!u)return true;
    // A parent can accept a tag even though its children reject it.
    if(s.size()>1&&s.size()%2)return false;
    s=string(s.size(),u);return true;
}
char compose(char f,char g){return f?f:g;}
char id(){return 0;}
using Tree=SegtreeBeats<string,op,e,char,mapping,compose,id,init>;
}
int main(){using namespace strings;mt19937 gen(905);
    original="abcdef";Tree forced(6);forced.apply(0,6,'z');
    assert(forced.t.size()==1);
    assert(forced.query(0,1)=="z"); // Push to a length-3 child fails.
    assert(forced.t.size()>3&&forced.query(0,6)=="zzzzzz");
    for(int trial=0;trial<100;++trial){
        int n=1+gen()%40;original=string(n,'a');
        for(char &c:original)c+=gen()%26;
        Tree t(n);string a=original;
        for(int step=0;step<300;++step){
            int l=gen()%(n+1),r=gen()%(n+1);if(l>r)swap(l,r);
            char c='a'+gen()%26;
            if(step%4==0){int p=gen()%n;t.set(p,string(1,c));a[p]=c;}
            else{t.apply(l,r,c);fill(a.begin()+l,a.begin()+r,c);}
            assert(t.query(l,r)==a.substr(l,r-l));
            assert(t.query(0,n)==a);
        }
    }
}
''')


if __name__ == '__main__':
    unittest.main()
