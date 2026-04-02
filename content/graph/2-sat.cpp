#include <vector>
#include <random>
#include <algorithm>
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

pair<int, vi> scc(vector<vi> &g) {
	int n = sz(g);
	vector<vi> rg(n);
	rep (v, 0, n) for (int u : g[v]) rg[u].pb(v);
	vi vis(n), ord, comp(n, -1);
	auto dfs1 = [&](auto self, int v) -> void {
		vis[v] = 1;
		for (int u : g[v]) if (!vis[u]) self(self, u);
		ord.pb(v);
	};
	auto dfs2 = [&](auto self, int v, int c) -> void {
		comp[v] = c;
		for (int u : rg[v]) if (comp[u] < 0) self(self, u, c);
	};
	rep (i, 0, n) if (!vis[i]) dfs1(dfs1, i);
	reverse(all(ord));
	int cnt = 0;
	for (int v : ord) if (comp[v] < 0) dfs2(dfs2, v, cnt++);
	return {cnt, comp};
}
// begin template //
struct TwoSat {
	int n;
	vector<vi> g;
	vi val;
	TwoSat(int n) : n(n), g(2 * n) {}
	int lit(int x, bool v) { return 2 * x + v; }
	void imp(int a, int b) { g[a].pb(b); }
	void clause(int a, bool va, int b, bool vb) {
		int x = lit(a, va), y = lit(b, vb);
		imp(x ^ 1, y);
		imp(y ^ 1, x);
	}
	bool solve() {
		auto [cnt, comp] = scc(g);
		val.assign(n, 0);
		rep (i, 0, n) {
			if (comp[2 * i] == comp[2 * i + 1]) return 0;
			val[i] = comp[2 * i] < comp[2 * i + 1];
		}
		return 1;
	}
};
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
	{ string t; cin >> t >> t; }
	int n, m;
	cin >> n >> m;
	TwoSat ts(n);
	rep (i, 0, m) {
		int a, b, zero;
		cin >> a >> b >> zero;
		ts.clause(abs(a) - 1, a > 0, abs(b) - 1, b > 0);
	}
	if (ts.solve()) {
		cout << "s SATISFIABLE" << endl;
		cout << "v ";
		rep(i, 0, n) {
			cout << (ts.val[i] ? i + 1: -i - 1) << ' ';
		}
		cout << 0 << endl;
	} else {
		cout << "s UNSATISFIABLE" << endl;
	}
}
