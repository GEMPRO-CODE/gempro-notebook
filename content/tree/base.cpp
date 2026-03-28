#include <vector>
#include <random>
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
// begin template //
struct Tree {
	int n, timer = 0;
	vector<vi> g;
	vi par, dep, cnt, head, in, out, rev;
	Tree(int n = 0)
		: n(n), g(n), par(n, -1), dep(n), cnt(n),
		  head(n), in(n), out(n), rev(n) {}
	void addEdge(int u, int v) {
		g[u].pb(v);
		g[v].pb(u);
	}
	void dfsSz(int v, int p = -1) {
		par[v] = p;
		cnt[v] = 1;
		if (p != -1) g[v].erase(find(all(g[v]), p));
		for (int &u : g[v]) {
			dep[u] = dep[v] + 1;
			dfsSz(u, v);
			cnt[v] += cnt[u];
			if (cnt[u] > cnt[g[v][0]]) swap(u, g[v][0]);
		}
	}
	void dfsHld(int v) {
		in[v] = timer;
		rev[timer++] = v;
		for (int u : g[v]) {
			head[u] = (u == g[v][0] ? head[v] : u);
			dfsHld(u);
		}
		out[v] = timer;
	}
	void build(int root = 0) {
		dfsSz(root);
		head[root] = root;
		dfsHld(root);
	}
};
// end template //
