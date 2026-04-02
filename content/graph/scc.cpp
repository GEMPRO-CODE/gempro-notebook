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

// begin template //
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

vector<vi> condGraph(vector<vi> &g, int cnt, vi &comp) {
	vector<vi> dag(cnt);
	rep (v, 0, sz(g)) for (int u : g[v])
		if (comp[v] != comp[u]) dag[comp[v]].pb(comp[u]);
	rep (i, 0, cnt) {
		sort(all(dag[i]));
		dag[i].erase(unique(all(dag[i])), end(dag[i]));
	}
	return dag;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
