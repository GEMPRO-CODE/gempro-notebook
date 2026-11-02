from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
COMPILER = shutil.which('g++')


@unittest.skipUnless(COMPILER, 'GNU C++ with PBDS required')
class PBDSTests(unittest.TestCase):
    def test_examples_and_containers(self):
        source = r'''
#include <cassert>
#include <map>
#include <random>
#include <set>
#include <vector>
#include "dsa/gnu-extensions.cpp"
int main() {
    pbdsExample();
    std::mt19937 rng(712);
    using P = std::pair<int,int>;
    OrderedTree<P> tree;
    std::set<P> reference;
    for (int i=0; i<3000; ++i) {
        P x{int(rng()%41)-20,i};
        tree.insert(x); reference.insert(x);
        if (rng()%2) {
            auto it=reference.begin(); std::advance(it,rng()%reference.size());
            tree.erase(*it); reference.erase(it);
        }
        int value=int(rng()%45)-22;
        assert(tree.order_of_key({value,-1}) ==
            size_t(std::distance(reference.begin(),reference.lower_bound({value,-1}))));
        int k=rng()%(reference.size()+1);
        auto want=reference.begin(); std::advance(want,k);
        auto got=tree.find_by_order(k);
        assert((got==tree.end())==(want==reference.end()));
        if (want!=reference.end()) assert(*got==*want);
    }
    OrderedTree<int,int,std::greater<int>> map;
    map[2]=20; map[4]=40; map[3]=30;
    assert(map.order_of_key(3)==1 && map.find_by_order(0)->second==40);
    HashTable<int,int> hash;
    HashTable<int> seen;
    for(int i=0;i<20000;++i) { hash[i]=3*i; seen.insert(i); }
    for(int i=0;i<20000;++i) {
        assert(hash.find(i)->second==3*i && seen.find(i)!=seen.end());
        if(i%2) {hash.erase(i); seen.erase(i);}
    }
    assert(hash.size()==10000 && seen.size()==10000);
    Heap<P> heap[2];
    std::set<P> expected[2];
    std::vector<Heap<P>::point_iterator> handles;
    std::vector<int> owner, value;
    for(int step=0;step<10000;++step) {
        int h=rng()%2, op=rng()%5;
        if(op==0 || expected[h].empty()) {
            int id=owner.size(), v=int(rng()%100)-50;
            handles.push_back(heap[h].push({v,id}));
            owner.push_back(h); value.push_back(v); expected[h].insert({v,id});
        } else if(op==1) {
            heap[h].join(heap[1-h]);
            for(auto x:expected[1-h]) {owner[x.second]=h; expected[h].insert(x);}
            expected[1-h].clear();
            assert(heap[1-h].empty());
        } else {
            auto it=expected[h].begin(); std::advance(it,rng()%expected[h].size());
            if(op==4) it=std::prev(expected[h].end());
            int id=it->second; expected[h].erase(it);
            if(op==2) {
                value[id]=int(rng()%200)-100;
                heap[h].modify(handles[id],{value[id],id});
                expected[h].insert({value[id],id});
                assert(*handles[id]==P(value[id],id));
            } else {
                if(op==3) heap[h].erase(handles[id]); else heap[h].pop();
                owner[id]=-1;
            }
        }
        for(int k=0;k<2;++k) {
            assert(heap[k].size()==expected[k].size());
            if(!heap[k].empty()) assert(heap[k].top()==*expected[k].rbegin());
        }
    }
    Heap<int,std::greater<int>> minheap;
    auto p=minheap.push(5); minheap.push(3); minheap.modify(p,1);
    assert(minheap.top()==1); minheap.pop(); assert(minheap.top()==3);
}
'''
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory)
            (path / 'test.cpp').write_text(source)
            result = subprocess.run(
                [COMPILER, '-std=c++20', '-O2', '-fsanitize=undefined',
                 '-fno-sanitize-recover=all', '-I', str(ROOT / 'content'),
                 str(path / 'test.cpp'), '-o', str(path / 'test')],
                capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            result = subprocess.run([str(path / 'test')], capture_output=True,
                                    text=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stderr)
