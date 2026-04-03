#include <vector>
#include <random>
#include <iostream>

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

// begin template //
vi domTree(vector<vi> &g, int s) {
	int n = sz(g), t = 0;
	vector<vi> rg(n + 1), bucket(n + 1);
	vi arr(n), rev(n + 1), par(n + 1), sdom(n + 1), dom(n + 1), dsu(n + 1), mn(n + 1);
	auto dfs = [&](auto &&self, int u) -> void {
		arr[u] = ++t; rev[t] = u;
		sdom[t] = dom[t] = dsu[t] = mn[t] = t;
		for (int v : g[u]) {
			if (!arr[v]) self(self, v), par[arr[v]] = arr[u];
			rg[arr[v]].pb(arr[u]);
		}
	};
	auto find = [&](auto &&self, int u, int x = 0) -> int {
		if (u == dsu[u]) return x ? -1 : u;
		int v = self(self, dsu[u], x + 1);
		if (v < 0) return u;
		if (sdom[mn[dsu[u]]] < sdom[mn[u]]) mn[u] = mn[dsu[u]];
		return dsu[u] = v, x ? v : mn[u];
	};
	dfs(dfs, s);
	for (int i = t; i; i--) {
		for (int j : rg[i]) sdom[i] = min(sdom[i], sdom[find(find, j)]);
		if (i > 1) bucket[sdom[i]].pb(i);
		for (int j : bucket[i]) {
			int y = find(find, j);
			dom[j] = sdom[y] == sdom[j] ? sdom[j] : y;
		}
		if (i > 1) dsu[i] = par[i];
	}
	rep(i, 2, t + 1) if (dom[i] != sdom[i]) dom[i] = dom[dom[i]];
	vi idom(n, -1);
	rep(i, 1, t + 1) idom[rev[i]] = rev[dom[i]];
	return idom;
}
// end template //

// Test at: https://judge.yosupo.jp/problem/dominatortree

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n, m, s;
	cin >> n >> m >> s;
	vector<vi> g(n);
	rep (i, 0, m) {
		int a, b;
		cin >> a >> b;
		g[a].pb(b);
	}
	auto dt = domTree(g, s);
	rep(i, 0, n) cout << dt[i] << ' ';
	cout << endl;
}
