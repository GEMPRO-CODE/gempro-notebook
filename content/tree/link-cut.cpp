#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

// begin template //
template<class S, S (*op)(S, S), S (*e)()>
struct LinkCut {
	struct Node { int c[2] = {}, p = 0; bool rev = 0;
		S val = e(), fwd = e(), bwd = e(); };
	vector<Node> t;
	LinkCut(int n) : t(n+1) {}
	bool auxRoot(int x) {
		int p = t[x].p;
		return !p || (t[p].c[0] != x && t[p].c[1] != x);
	}
	void pull(int x) {
		auto &a = t[t[x].c[0]], &b = t[t[x].c[1]];
		t[x].fwd = op(op(a.fwd,t[x].val),b.fwd);
		t[x].bwd = op(op(b.bwd,t[x].val),a.bwd);
	}
	void flip(int x) {
		if (x) {
			swap(t[x].c[0],t[x].c[1]);
			swap(t[x].fwd,t[x].bwd); t[x].rev ^= 1;
		}
	}
	void push(int x) {
		if (t[x].rev) flip(t[x].c[0]), flip(t[x].c[1]), t[x].rev = 0;
	}
	void rotate(int x) {
		int p = t[x].p, g = t[p].p, d = t[p].c[1] == x;
		int b = t[x].c[d^1];
		if (!auxRoot(p)) t[g].c[t[g].c[1] == p] = x;
		t[x].p = g; t[x].c[d^1] = p; t[p].p = x;
		t[p].c[d] = b; if (b) t[b].p = p;
		pull(p); pull(x);
	}
	void splay(int x) {
		vector<int> path{x};
		for (int y = x; !auxRoot(y); ) path.push_back(y = t[y].p);
		for (auto it = path.rbegin(); it != path.rend(); ++it) push(*it);
		while (!auxRoot(x)) {
			int p = t[x].p, g = t[p].p;
			if (!auxRoot(p)) rotate((t[p].c[1] == x) ==
				(t[g].c[1] == p) ? p : x);
			rotate(x);
		}
	}
	void access(int x) {
		for (int y = x, last = 0; y; last = y, y = t[y].p) {
			splay(y); t[y].c[1] = last; pull(y);
		}
		splay(x);
	}
	void makeRoot(int x) { access(x); flip(x); }
	int root(int x) {
		access(x); push(x);
		while (t[x].c[0]) x = t[x].c[0], push(x);
		splay(x); return x;
	}
	bool connected(int u, int v) { return root(u) == root(v); }
	bool link(int u, int v) {
		makeRoot(u); if (root(v) == u) return false;
		t[u].p = v; return true;
	}
	bool cut(int u, int v) {
		makeRoot(u); access(v);
		if (t[v].c[0] != u || t[u].c[1]) return false;
		t[v].c[0] = t[u].p = 0; pull(v); return true;
	}
	void set(int u, S val) { access(u); t[u].val = val; pull(u); }
	S query(int u, int v) {
		makeRoot(u); access(v); return t[v].fwd;
	}
};
// end template //

// Test at https://judge.yosupo.jp/problem/dynamic_tree_vertex_set_path_composite

const ll MOD = 998244353;
struct Affine { ll a, b; };
Affine compose(Affine f, Affine g) { // f first, then g
	return {g.a*f.a%MOD, (g.a*f.b+g.b)%MOD};
}
Affine affineIdentity() { return {1, 0}; }
using PathComposite = LinkCut<Affine, compose, affineIdentity>;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int n, q;
	cin >> n >> q;
	PathComposite t(n);
	for (int i = 1; i <= n; ++i) {
		ll a, b; cin >> a >> b;
		t.set(i, {a, b});
	}
	for (int i = 1; i < n; ++i) {
		int u, v; cin >> u >> v;
		t.link(u+1, v+1);
	}
	while (q--) {
		int type; cin >> type;
		if (type == 0) {
			int u, v, w, x; cin >> u >> v >> w >> x;
			t.cut(u+1, v+1); t.link(w+1, x+1);
		} else if (type == 1) {
			int p; ll a, b; cin >> p >> a >> b;
			t.set(p+1, {a, b});
		} else {
			int u, v; ll x; cin >> u >> v >> x;
			auto f = t.query(u+1, v+1);
			cout << (f.a*x+f.b)%MOD << '\n';
		}
	}
}
