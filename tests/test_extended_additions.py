import json
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
COMPILER = shutil.which('g++') or shutil.which('clang++')


@unittest.skipUnless(COMPILER, 'C++ compiler required')
class ExtendedAdditionsTests(unittest.TestCase):
    def run_cpp(self, path, body):
        source = '#include <algorithm>\n#include <cassert>\n#include <climits>\n#include <map>\n#include <numeric>\n#include <random>\n#include <string>\n'
        source += '#include '+json.dumps(str(ROOT/'content'/path))+'\n'+body
        with tempfile.TemporaryDirectory() as directory:
            p = Path(directory)
            (p/'test.cpp').write_text(source)
            result = subprocess.run([COMPILER,'-std=c++20','-O2',str(p/'test.cpp'),'-o',str(p/'test')],capture_output=True,text=True)
            self.assertEqual(result.returncode,0,result.stderr)
            result = subprocess.run([str(p/'test')],capture_output=True,text=True,timeout=45)
            self.assertEqual(result.returncode,0,result.stderr)

    def test_manhattan_candidates_against_complete_graph_prim(self):
        self.run_cpp('geometry/manhattan-mst.cpp',r'''
ll distance(Pt a,Pt b){return abs(a.xx-b.xx)+abs(a.yy-b.yy);}
void check(vector<Pt> points){
    int n=sz(points);auto original=points;auto es=manhattanMST(points);
    assert(points==original&&sz(es)<=4*n);
    for(auto [w,u,v]:es){assert(0<=u&&u<n&&0<=v&&v<n&&u!=v);assert(w==distance(points[u],points[v]));}
    // Dense Prim on the original complete graph, independent of the sweep.
    vector<ll> d(n,LLONG_MAX);vi seen(n);__int128 expected=0;
    if(n)d[0]=0;
    for(int step=0;step<n;++step){
        int u=-1;for(int i=0;i<n;++i)if(!seen[i]&&(u<0||d[i]<d[u]))u=i;
        seen[u]=1;expected+=d[u];
        for(int v=0;v<n;++v)if(!seen[v])d[v]=min(d[v],distance(points[u],points[v]));
    }
    sort(all(es));vi par(n);iota(all(par),0);
    auto root=[&](int u){while(u!=par[u])u=par[u];return u;};
    __int128 got=0;int chosen=0;
    for(auto [w,u,v]:es){u=root(u);v=root(v);if(u!=v){par[u]=v;got+=w;++chosen;}}
    assert(chosen==max(0,n-1)&&got==expected);
}
int main(){
    vector<Pt> grid;for(int x=-1;x<=1;++x)for(int y=-1;y<=1;++y)grid.emplace_back(x,y);
    vector<Pt> points;
    auto generate=[&](auto &&self,int first,int left)->void{
        check(points);if(!left)return;
        for(int i=first;i<sz(grid);++i){points.push_back(grid[i]);self(self,i,left-1);points.pop_back();}
    };generate(generate,0,5);
    const ll B=1000000000000000000LL;
    check({{-B,-B},{B,B},{-B,B},{B,-B},{0,0},{0,0}});
    mt19937_64 gen(2026);
    for(int trial=0;trial<1000;++trial){
        int n=gen()%45;points.clear();
        for(int i=0;i<n;++i){ll x=(ll)(gen()%101)-50,y=(ll)(gen()%101)-50;
            if(trial%4==0)y=x;if(trial%4==1)y=-x;if(trial%4==2)y=0;
            if(trial%7==0)x*=10000000000000000LL,y*=10000000000000000LL;
            points.emplace_back(x,y);}
        shuffle(all(points),gen);check(points);
    }
}
''')

    def test_eertree_distinct_palindromes_counts_and_suffix_links(self):
        self.run_cpp('strings/palindromic-tree.cpp',r'''
bool palindrome(string s){string r=s;reverse(r.begin(),r.end());return r==s;}
void check(string text){
    Eertree tree;string prefix;
    assert(tree.occurrences()==vi({0,0}));
    for(char c:text){
        prefix+=c;int last=tree.add(c),longest=0;map<string,int> want;
        for(int l=0;l<(int)prefix.size();++l)for(int r=l+1;r<=(int)prefix.size();++r){
            string p=prefix.substr(l,r-l);if(palindrome(p)){++want[p];if(r==(int)prefix.size())longest=max(longest,r-l);}}
        assert(tree.t[last].len==longest&&last==tree.last);
        assert(tree.t.size()==want.size()+2);
        auto count=tree.occurrences();assert(tree.occurrences()==count);
        map<string,int> got;
        for(int v=2;v<(int)tree.t.size();++v){auto node=tree.t[v];
            string p=prefix.substr(node.end-node.len+1,node.len);
            assert(palindrome(p)&&!got.count(p));got[p]=count[v];
            int best=0;for(int k=1;k<node.len;++k)if(palindrome(p.substr(node.len-k)))best=k;
            assert(tree.t[node.link].len==best&&node.link<v);
            if(node.link>=2){auto link=tree.t[node.link];
                assert(prefix.substr(link.end-link.len+1,link.len)==p.substr(p.size()-best));}
        }
        assert(got==want);
    }
}
int main(){
    check("");check("a");check("abacaba");check("aaaaaaa");check("abababababab");
    for(int n=1;n<=9;++n)for(int mask=0;mask<(1<<n);++mask){string s;
        for(int i=0;i<n;++i)s+='a'+(mask>>i&1);check(s);}
    mt19937 gen(2026);
    for(int trial=0;trial<200;++trial){string s;
        for(int i=0,n=gen()%35;i<n;++i)s+='a'+gen()%4;check(s);}
    Eertree tree;int n=100000;
    for(int i=0;i<n;++i)tree.add('z');
    auto count=tree.occurrences();assert(tree.t.size()==n+2);
    for(int v=2;v<n+2;++v)assert(count[v]==n-tree.t[v].len+1);
}
''')

    def test_edge_coloring_exhaustive_and_adversarial_graphs(self):
        self.run_cpp('graph/edge-coloring.cpp',r'''
void check(int n,vector<pii> es){
    auto colors=edgeColoring(n,es);assert(colors.size()==es.size());
    vi degree(n);for(auto [u,v]:es)++degree[u],++degree[v];
    int delta=n?*max_element(all(degree)):0;
    vector<vi> used(n,vi(delta+1));
    for(int i=0;i<sz(es);++i){auto [u,v]=es[i];int c=colors[i];
        assert(0<=c&&c<=delta);assert(!used[u][c]&&!used[v][c]);used[u][c]=used[v][c]=1;}
}
int main(){
    mt19937 gen(2026);check(0,{});
    for(int n=1;n<=6;++n){vector<pii> allEdges;
        for(int u=0;u<n;++u)for(int v=u+1;v<n;++v)allEdges.emplace_back(u,v);
        for(int mask=0;mask<(1<<sz(allEdges));++mask){vector<pii> es;
            for(int i=0;i<sz(allEdges);++i)if(mask>>i&1){auto e=allEdges[i];if(gen()%2)swap(e.first,e.second);es.push_back(e);}
            shuffle(all(es),gen);check(n,es);}}
    for(int n:{7,16,35,70})for(int kind=0;kind<5;++kind){vector<pii> es;
        for(int u=0;u<n;++u)for(int v=u+1;v<n;++v)
            if(kind==0||(kind==1&&u==0)||(kind==2&&((u<n/2)!=(v<n/2)))||(kind==3&&(v==u+1||(u==0&&v==n-1)))||(kind==4&&gen()%4==0))es.emplace_back(u,v);
        for(int t=0;t<5;++t){shuffle(all(es),gen);check(n,es);}}
}
''')


if __name__ == '__main__':
    unittest.main()
