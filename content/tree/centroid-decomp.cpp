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
#define rep(i,a,b) for (int i = (a); i < (b); i++)

mt19937 rng(random_device{}());

// begin template //
vi centroidDecomp(vector<vi> &g) {
	int n = sz(g);
	vi par(n, -1), sub(n), dead(n);
	auto dfs = [&](auto &&self, int v, int p) -> int {
		sub[v] = 1;
		for (int u : g[v]) if (u != p && !dead[u])
			sub[v] += self(self, u, v);
		return sub[v];
	};
	auto cen = [&](auto &&self, int v, int p, int m) -> int {
		for (int u : g[v]) if (u != p && !dead[u] && sub[u] > m / 2)
			return self(self, u, v, m);
		return v;
	};
	auto build = [&](auto &&self, int v, int p) -> void {
		int m = dfs(dfs, v, -1), c = cen(cen, v, -1, m);
		par[c] = p, dead[c] = 1;
		for (int u : g[c]) if (!dead[u]) self(self, u, c);
	};
	build(build, 0, -1);
	return par;
}
// end template //

// Test at: https://atcoder.jp/contests/abc291/tasks/abc291_h

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n;
	cin >> n;
	vector<vector<int>> g(n);
	for (int i = 1; i < n; i++) {
		int a, b;
		cin >> a >> b;
		a--, b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	auto par = centroidDecomp(g);
	for (int i = 0; i < n; i++) {
		if (par[i] == -1) cout << -1 << ' ';
		else cout << par[i] + 1 << ' ';
	}
	cout << endl;
}
