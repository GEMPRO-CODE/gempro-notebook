#include <vector>
#include <algorithm>
#include <iostream>
#include <random>

using namespace std;

using ll = long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;

#define pb push_back
#define eb emplace_back
#define fi first
#define se second
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)

mt19937 rng(random_device{}());
template <class S, S (*op)(S, S)> struct SparseTable {
	vector<vector<S>> t;
	vi lg;
	void build(vector<S> &v) {
		int n = ssize(v);
		lg.assign(n + 1, 0);
		for (int i = 2; i <= n; i++) lg[i] = lg[i >> 1] + 1;
		t.assign(lg[n] + 1, vector<S>(n));
		t[0] = v;
		for (int k = 1; k <= lg[n]; k++)
			for (int i = 0; i + (1 << k) <= n; i++)
				t[k][i] = op(t[k - 1][i], t[k - 1][i + (1 << (k - 1))]);
	}
	int query(int l, int r) {
		int k = lg[r - l];
		return op(t[k][l], t[k][r - (1 << k)]);
	}
};


// begin template //

using RMQ = SparseTable<int, [](int a, int b) { return min(a, b); }>;

struct LCA {
	int t = 0;
	vi tm, path, ret;
	RMQ rmq;
	LCA(vector<vi> &g, int r = 0) : tm(g.size()) {
		dfs(g, r, -1);
		rmq.build(ret);
	}
	void dfs(vector<vi> &g, int x, int pre) {
		tm[x] = t++;
		for (int y: g[x]) if (y != pre) {
			path.pb(x), ret.pb(tm[x]);
			dfs(g, y, x);
		}
	}
	int lca(int x, int y) {
		if (x == y) return x;
		tie(x, y) = minmax(tm[x], tm[y]);
		return path[rmq.query(x, y)];
	}
};

// end template //

// Test at https://judge.yosupo.jp/problem/lca

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n, q;
	cin >> n >> q;
	vector<vi> adj(n);
	for (int i = 1; i < n; i++) {
		int t;
		cin >> t;
		adj[t].push_back(i);
		adj[i].push_back(t);
	}
	LCA lca(adj);
	for (int i = 0; i < q; i++) {
		int a, b;
		cin >> a >> b;
		cout << lca.lca(a, b) << endl;
	}
	return 0;
}
