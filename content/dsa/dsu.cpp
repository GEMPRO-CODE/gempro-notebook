#include <vector>
#include <numeric>
using namespace std;

using VI = vector<int>;

// begin template //
struct DSU {
	VI par, cnt;
	DSU(int n): par(n), cnt(n, 1) {
		iota(par.begin(), par.end(), 0);
	}
	int root(int x) {
		if (par[x] == x) return x;
		return par[x] = root(par[x]);
	}
	bool merge(int x, int y) {
		x = root(x), y = root(y);
		if (x == y) return false;
		if (cnt[x] < cnt[y]) swap(x, y);
		cnt[x] += cnt[y], par[y] = x;
		return true;
	}
};
// end template //
