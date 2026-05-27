#include <vector>
#include <random>
#include <iostream>
#include <algorithm>

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
		int n = sz(v);
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

// begin template //
vector<pii> virtree(vector<vi> &t, LCA &lca, vi &tin, vi &tout, vi &v) {
	if (v.empty()) return {};
	auto cmp = [&](int a, int b) { return tin[a] < tin[b]; };
	sort(all(v), cmp);
	int m = sz(v);
	rep(i,0,m-1) v.pb(lca.lca(v[i], v[i+1]));
	sort(all(v), cmp);
	v.erase(unique(all(v)), v.end());
	vector<pii> e;
	vi st = {v[0]};
	rep(i,1,sz(v)) {
		while (tout[st.back()] <= tin[v[i]]) st.pop_back();
		e.eb(st.back(), v[i]);
		st.pb(v[i]);
	}
	return e;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
