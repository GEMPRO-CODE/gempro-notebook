#include <vector>
#include <iostream>

using namespace std;

using VI = vector<int>;
using VVI = vector<VI>;

struct Tree {
	VVI adj;
	VI cnt, par, dst, tin, tout, nxt;
	int t;
	Tree(int n): adj(n), cnt(n), par(n), dst(n), tin(n), tout(n), nxt(n), t(0) {}
	void addEdge(int a, int b) { adj[a].push_back(b); adj[b].push_back(a); }
	void build(int r = 0) { build(r, -1); par[r] = r; nxt[r] = r; prepare(r); }
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

// begin template //
struct TreeMove {
	int n;
	VI w, tm;
	Tree &t;
	TreeMove(Tree &t): n(t.adj.size()), w(n), tm(n), t(t) {
		for (int i = 0; i < n; i++) {
			w[t.tin[i]] = i;
			tm[i] = t.tin[i];
		}
	}
	int lca(int x, int y) {
		while (t.nxt[x] != t.nxt[y]) {
			if (t.dst[t.nxt[x]] > t.dst[t.nxt[y]]) x = t.par[t.nxt[x]];
			else y = t.par[t.nxt[y]];
		}
		return w[min(tm[x], tm[y])];
	}
	int up(int x, int d) {
		while (t.dst[x] - t.dst[t.nxt[x]] < d) {
			d -= t.dst[x] - t.dst[t.nxt[x]] + 1;
			x = t.par[t.nxt[x]];
		}
		return w[tm[x] - d];
	}
	int move(int x, int y, int d) {
		int l = lca(x, y), ld = t.dst[l], dx = t.dst[x] - ld, dy = t.dst[y] - ld;
		return d > dx ? up(y, dx + dy - d) : up(x, d);
	}
	int dist(int x, int y) {
		return t.dst[x] + t.dst[y] - 2 * t.dst[lca(x, y)];
	}
};
// end template //

// Test at https://judge.yosupo.jp/problem/jump_on_tree

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n, q;
	cin >> n >> q;
	Tree t(n);
	for (int i = 1; i < n; i++) {
		int a, b;
		cin >> a >> b;
		t.addEdge(a, b);
	}
	t.build();
	TreeMove tmv(t);
	while (q--) {
		int s, t, d;
		cin >> s >> t >> d;
		if (tmv.dist(s, t) < d) {
			cout << -1 << '\n';
			continue;
		}
		cout << tmv.move(s, t, d) << endl;
	}
	return 0;
}
