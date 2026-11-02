from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
COMPILER = shutil.which('g++')


@unittest.skipUnless(COMPILER, 'g++ required')
class MoreSubproblemsTests(unittest.TestCase):
    def run_cpp(self, name, body):
        source = '#include <bits/stdc++.h>\n#include <cassert>\n'
        source += f'#include "{name}.cpp"\n' + body
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
                                    text=True, timeout=60)
            self.assertEqual(result.returncode, 0, result.stderr)

    def test_distinct_intervals(self):
        self.run_cpp('distinct-intervals', r'''
bool brute(const vector<pair<ll,ll>> &a,int i=0,int used=0) {
    if(i==int(a.size())) return true;
    for(ll x=a[i].first;x<=a[i].second;++x)
        if(!(used>>(x+3)&1) && brute(a,i+1,used|(1<<(x+3)))) return true;
    return false;
}
void check(const vector<pair<ll,ll>> &a,bool possible) {
    auto ans=distinctIntervals(a); assert(bool(ans)==possible);
    if(!ans) return;
    set<ll> used;
    assert(ans->size()==a.size());
    for(int i=0;i<int(a.size());++i) {
        assert(a[i].first<=(*ans)[i] && (*ans)[i]<=a[i].second);
        assert(used.insert((*ans)[i]).second);
    }
}
int main() {
    mt19937 gen(602);
    for(int n=0;n<=8;++n) for(int t=0;t<500;++t) {
        vector<pair<ll,ll>> a(n);
        for(auto &[l,r]:a) { l=int(gen()%7)-3; r=int(gen()%7)-3; }
        check(a,brute(a));
    }
    check({{LLONG_MIN,LLONG_MIN},{LLONG_MAX,LLONG_MAX}},true);
    check({{LLONG_MAX,LLONG_MAX},{LLONG_MAX,LLONG_MAX}},false);
    check({{LLONG_MAX-1,LLONG_MAX},{LLONG_MAX-1,LLONG_MAX}},true);
    check({{LLONG_MIN,LLONG_MAX},{0,0},{1,1}},true);
    check({{5,4}},false);
    vector<pair<ll,ll>> a(100000,{0,99999});check(a,true);
}
''')

    def test_lis_3d(self):
        self.run_cpp('lis-3d', r'''
bool less3(array<ll,3> a,array<ll,3> b) {
    return a[0]<b[0] && a[1]<b[1] && a[2]<b[2];
}
void check(vector<array<ll,3>> p,vector<ll> w={}) {
    auto original=p;
    auto ans=lis3D(p,w); assert(p==original);
    int n=p.size();if(w.empty())w.assign(n,1);
    vector<int> ord(n);iota(ord.begin(),ord.end(),0);
    sort(ord.begin(),ord.end(),[&](int a,int b){return p[a]<p[b];});
    vector<ll> dp=w;ll best=LLONG_MIN;
    for(int i:ord) {
        for(int j:ord) if(less3(p[j],p[i])) dp[i]=max(dp[i],dp[j]+w[i]);
        best=max(best,dp[i]);
    }
    assert(ans.empty()==p.empty());
    set<int> used;ll score=0;
    for(int i=0;i<int(ans.size());++i) {
        assert(ans[i]>=0 && ans[i]<n);assert(used.insert(ans[i]).second);
        if(i)assert(less3(p[ans[i-1]],p[ans[i]]));
        score+=w[ans[i]];
    }
    if(n)assert(score==best);
}
int main() {
    mt19937 gen(603);
    check({});check({{1,1,1}});
    vector<array<ll,3>> cube;
    for(int x=0;x<2;++x)for(int y=0;y<2;++y)for(int z=0;z<2;++z)cube.push_back({x,y,z});
    for(int mask=0;mask<256;++mask) {
        vector<array<ll,3>> p;
        for(int i=0;i<8;++i)if(mask>>i&1)p.push_back(cube[i]);
        shuffle(p.begin(),p.end(),gen);check(p);
    }
    for(int t=0;t<2000;++t) {
        vector<array<ll,3>> p(gen()%80);
        for(auto &a:p)for(auto &v:a)v=int(gen()%9)-4;
        check(p);
        vector<ll> w(p.size());for(auto &v:w)v=int(gen()%31)-15;
        check(p,w);
    }
    check({{0,0,0},{1,1,1},{2,2,2}}, {-9,-2,-7});
    check({{0,0,0},{1,1,1},{2,2,2}}, {0,0,0});
    check({{0,0,0},{1,1,1},{2,2,2}}, {8,-100,9});
    check({{0,0,0},{1,1,1},{2,2,2}}, {4000000000LL,5000000000LL,6000000000LL});
    check({{0,0,0}}, {LLONG_MIN});
    check({{0,0,0}}, {LLONG_MAX});
    for(int axis=0;axis<3;++axis) {
        vector<array<ll,3>> p;
        for(int i=0;i<100;++i) { array<ll,3>a{i,i,i};a[axis]=0;p.push_back(a); }
        shuffle(p.begin(),p.end(),gen);check(p);
    }
    check(vector<array<ll,3>>(100,{1,2,3}));
    check({{LLONG_MAX,LLONG_MAX,LLONG_MAX},{0,0,0},{LLONG_MIN,LLONG_MIN,LLONG_MIN}});
    int n=100000;vector<array<ll,3>> p;
    for(int i=0;i<n;++i)p.push_back({i,i,i});
    shuffle(p.begin(),p.end(),gen);auto ans=lis3D(p);assert(int(ans.size())==n);
    for(int i=1;i<n;++i)assert(less3(p[ans[i-1]],p[ans[i]]));
}
''')

    def test_largest_rectangle(self):
        self.run_cpp('largest-rectangle', r'''
void check(const vector<vector<int>> &a) {
    int n=a.size(),m=n?a[0].size():0;
    auto [r1,c1,r2,c2]=largestRectangle(a);
    assert(0<=r1 && r1<=r2 && r2<=n && 0<=c1 && c1<=c2 && c2<=m);
    for(int i=r1;i<r2;++i)for(int j=c1;j<c2;++j)assert(a[i][j]);
    int best=0;
    for(int l=0;l<n;++l)for(int r=l+1;r<=n;++r)
    for(int x=0;x<m;++x)for(int y=x+1;y<=m;++y) {
        bool ok=true;for(int i=l;i<r;++i)for(int j=x;j<y;++j)ok &= a[i][j]!=0;
        if(ok)best=max(best,(r-l)*(y-x));
    }
    assert((r2-r1)*(c2-c1)==best);
    if(!best)assert((array<int,4>{r1,c1,r2,c2}==array<int,4>{}));
}
int main() {
    check({});check(vector<vector<int>>(4));
    for(int mask=0;mask<(1<<16);++mask) {
        vector<vector<int>> a(4,vector<int>(4));
        for(int i=0;i<16;++i)a[i/4][i%4]=(mask>>i&1)?-2:0;
        check(a);
    }
    mt19937 gen(604);
    for(int t=0;t<500;++t) {
        vector<vector<int>> a(1+gen()%7,vector<int>(1+gen()%7));
        for(auto &r:a)for(auto &v:r)v=gen()%2;
        check(a);
    }
}
''')
