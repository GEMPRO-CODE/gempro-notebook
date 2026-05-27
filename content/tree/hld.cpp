#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <cassert>

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

const int mod = 998244353;

// begin template //
struct HLD {
	vi cnt, nxt, par, tin, tout, d;
	int T = 0;
	HLD(vector<vi> g, int r = 0) :
		cnt(sz(g)), nxt(sz(g)), par(sz(g), -1),
		tin(sz(g)), tout(sz(g)), d(sz(g)) {
		auto dfs1 = [&](auto &&self, int u) -> void {
			cnt[u] = 1, nxt[u] = u;
			if (par[u] != -1) g[u].erase(find(all(g[u]), par[u]));
			for (int &v: g[u]) {
				par[v] = u, d[v] = d[u] + 1;
				self(self, v);
				cnt[u] += cnt[v];
				if (cnt[v] > cnt[g[u][0]]) swap(v, g[u][0]);
			}
		};
		auto dfs2 = [&](auto &&self, int u) -> void {
			tin[u] = T++;
			for (int v: g[u]) {
				if (v == g[u][0]) nxt[v] = nxt[u];
				self(self, v);
			}
			tout[u] = T;
		};
		dfs1(dfs1, r), dfs2(dfs2, r);
	}
	vector<pii> path(int u, int v) {
		vector<pii> a, b;
		while (nxt[u] != nxt[v]) {
			if (d[nxt[u]] < d[nxt[v]])
				b.eb(tin[nxt[v]], tin[v]), v = par[nxt[v]];
			else
				a.eb(tin[u], tin[nxt[u]]), u = par[nxt[u]];
		}
		(d[u] < d[v] ? b : a).eb(tin[u], tin[v]);
		a.insert(end(a), rbegin(b), rend(b));
		return a;
	}
};
// end template //

// Test at: https://judge.yosupo.jp/problem/vertex_set_path_composite

struct Affine {
	ll a = 1, b = 0;
	ll operator()(ll x) {
		return (a * x + b) % mod;
	}
};

Affine compose(Affine f, Affine g) {
	return {g.a * f.a % mod, (g.a * f.b + g.b) % mod};
}

struct Segtree {
	vector<Affine> t;
	int n;
	Segtree(int N): t(2 * N), n(N) {}
	void set(int i, Affine f) {
		t[i += n] = f;
		while (i >>= 1) t[i] = compose(t[i << 1], t[i << 1 | 1]);
	}
	Affine query(int l, int r) {
		Affine al, ar;
		for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
			if (l & 1) al = compose(al, t[l++]);
			if (r & 1) ar = compose(t[--r], ar);
		}
		return compose(al, ar);
	}
};

struct RevSegtree {
	vector<Affine> t;
	int n;
	RevSegtree(int N): t(2 * N), n(N) {}
	void set(int i, Affine f) {
		t[i += n] = f;
		while (i >>= 1) t[i] = compose(t[i << 1 | 1], t[i << 1]);
	}
	Affine query(int l, int r) {
		Affine al, ar;
		for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
			if (l & 1) al = compose(t[l++], al);
			if (r & 1) ar = compose(ar, t[--r]);
		}
		return compose(ar, al);
	}
};

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n, q;
	cin >> n >> q;
	Segtree seg(n);
	RevSegtree rev(n);
	vector<vector<int>> g(n);
	vector<Affine> f(n);
	for (int i = 0; i < n; i++) {
		cin >> f[i].a >> f[i].b;
	}
	for (int i = 1; i < n; i++) {
		int u, v;
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	HLD hld(g);
	for (int i = 0; i < n; i++) {
		seg.set(hld.tin[i], f[i]);
		rev.set(hld.tin[i], f[i]);
	}
	while (q--) {
		int t;
		cin >> t;
		if (t == 0) {
			int p, c, d;
			cin >> p >> c >> d;
			f[p] = {c, d};
			seg.set(hld.tin[p], f[p]);
			rev.set(hld.tin[p], f[p]);
		} else if (t == 1) {
			int u, v, x;
			cin >> u >> v >> x;
			auto path = hld.path(u, v);
			Affine f;
			for (auto [l, r]: path) {
				if (l > r) {
					f = compose(f, rev.query(r, l + 1));
				} else {
					f = compose(f, seg.query(l, r + 1));
				}
			}
			cout << f(x) << endl;
		} else assert(false);
	}
}
