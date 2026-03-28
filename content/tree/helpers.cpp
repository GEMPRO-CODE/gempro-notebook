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
// begin template //
bool isAnc(Tree& t, int a, int b) {
	return t.in[a] <= t.in[b] && t.out[b] <= t.out[a];
}
pii subtree(Tree& t, int v) {
	return {t.in[v], t.out[v]};
}
int lca(Tree& t, int a, int b) {
	while (t.head[a] != t.head[b]) {
		if (t.dep[t.head[a]] > t.dep[t.head[b]]) a = t.par[t.head[a]];
		else b = t.par[t.head[b]];
	}
	return t.dep[a] < t.dep[b] ? a : b;
}
int dist(Tree& t, int a, int b) {
	int c = lca(t, a, b);
	return t.dep[a] + t.dep[b] - 2 * t.dep[c];
}
int kthAnc(Tree& t, int v, int k) {
	while (v != -1) {
		int h = t.head[v], d = t.dep[v] - t.dep[h];
		if (k <= d) return t.rev[t.in[v] - k];
		k -= d + 1;
		v = t.par[h];
	}
	return -1;
}
// k-th vertex on path a -> b, 0-indexed
int jump(Tree& t, int a, int b, int k) {
	int c = lca(t, a, b);
	int da = t.dep[a] - t.dep[c], db = t.dep[b] - t.dep[c];
	if (k < 0 || k > da + db) return -1;
	if (k <= da) return kthAnc(t, a, k);
	return kthAnc(t, b, da + db - k);
}
void getPath(Tree& t, int a, int b, auto f) {
	while (t.head[a] != t.head[b]) {
		if (t.dep[t.head[a]] > t.dep[t.head[b]]) swap(a, b);
		f(t.in[t.head[b]], t.in[b] + 1);
		b = t.par[t.head[b]];
	}
	if (t.dep[a] > t.dep[b]) swap(a, b);
	f(t.in[a], t.in[b] + 1);
}
// end template //
