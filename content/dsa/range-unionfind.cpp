#include <vector>
#include <algorithm>
#include <bit>
using namespace std;
using vi = vector<int>;

// begin template //
struct RangeUnionFind {
	vector<vi> p;
	RangeUnionFind(int n): p(bit_width(unsigned(n))) {
		for (int k = 0; k < int(p.size()); ++k)
			p[k].assign(n-(1<<k)+1,-1);
	}
	int root(int x, int k = 0) {
		return p[k][x] < 0 ? x : p[k][x] = root(p[k][x],k);
	}
	void join(int a, int b, int k, auto &f) {
		int x = root(a,k), y = root(b,k);
		if (x == y) return;
		if (p[k][x] > p[k][y]) swap(x,y);
		p[k][x] += p[k][y]; p[k][y] = x;
		if (!k) { f(x,y); return; }
		int d = 1<<(k-1);
		join(a,b,k-1,f); join(a+d,b+d,k-1,f);
	}
	void merge(int a, int b, int len, auto f) {
		if (!len) return;
		int k = bit_width(unsigned(len))-1, d = len-(1<<k);
		join(a,b,k,f); join(a+d,b+d,k,f);
	}
};
// end template //
