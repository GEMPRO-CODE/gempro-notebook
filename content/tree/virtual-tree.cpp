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

int lca(Tree& t, int a, int b) {
	while (t.head[a] != t.head[b]) {
		if (t.dep[t.head[a]] > t.dep[t.head[b]]) a = t.par[t.head[a]];
		else b = t.par[t.head[b]];
	}
	return t.dep[a] < t.dep[b] ? a : b;
}

// begin template //
vector<pii> virtree(Tree &t, vi &v) {
	if (v.empty()) return {};
	auto cmp = [&](int a, int b) { return t.in[a] < t.in[b]; };
	sort(all(v), cmp);
	int m = sz(v);
	rep(i,0,m-1) v.pb(lca(t, v[i], v[i+1]));
	sort(all(v), cmp);
	v.erase(unique(all(v)), v.end());
	vector<pii> e;
	vi st = {v[0]};
	rep(i,1,sz(v)) {
		while (t.out[st.back()] <= t.in[v[i]]) st.pop_back();
		e.eb(st.back(), v[i]);
		st.pb(v[i]);
	}
	return e;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
