#include <vector>

using namespace std;

using AL = vector<vector<int>>;

// begin template //

struct Tree {
	AL adj;
	vector<int> cnt, par, dst, tin, tout, nxt;
	int t;
	Tree(int n): adj(n), cnt(n), par(n), dst(n), tin(n), tout(n), nxt(n), t(0) {}
	void addEdge(int a, int b) { adj[a].push_back(b), adj[b].push_back(a); }
	void build(int r = 0) { build(r, -1); prepare(r); }
	void build(int x, int pre) {
		cnt[x] = 1;
		for (int &y: adj[x]) if (y != pre) {
			par[y] = x, dst[y] = dst[x] + 1;
			build(y, x);
			cnt[x] += cnt[y];
			if (cnt[y] > cnt[adj[x][0]]) swap(adj[x][0], y);
		}
	}
	void prepare(int x) {
		tin[x] = t++;
		for (int y: adj[x]) if (y != par[x]) {
			nxt[y] = y == adj[x][0] ? nxt[x] : y;
			prepare(y);
		}
		tout[x] = t;
	}
};

// end template //
