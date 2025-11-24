#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

using i64 = long long;
using PII = pair<int, int>;
using VI = vector<int>;
using VVI = vector<VI>;
using VII = vector<PII>;

struct Tree {
	VVI adj;
	VI cnt, par, dst, tin, tout, nxt;
	int t;
	Tree(int n): adj(n), cnt(n), par(n), dst(n), tin(n), tout(n), nxt(n), t(0) {}
	void addEdge(int a, int b) { adj[a].push_back(b), adj[b].push_back(a); }
	void build(int r = 0) {
		build(r, -1);
		prepare(r);
	}
	void build(int x, int pre) {
		cnt[x] = 1;
		for (int &y: adj[x]) if (y != pre) {
			par[y] = x, dst[y] = dst[x] + 1;
			build(y, x);
			cnt[x] += cnt[y];
			if (cnt[y] > cnt[adj[x][0]]) swap(adj[x][0], y);
		}
	}
	void prepare(int x) {
		tin[x] = t++;
		for (int y: adj[x]) if (y != par[x]) {
			nxt[y] = y == adj[x][0] ? nxt[x] : y;
			prepare(y);
		}
		tout[x] = t;
	}
};

// begin template //

VII hldPath(Tree &t, int x, int y, bool ord = false) {
	VII p, rp;
	while (t.nxt[x] != t.nxt[y]) {
		if (t.dst[t.nxt[x]] < t.dst[t.nxt[y]]) {
			rp.emplace_back(t.tin[t.nxt[y]], t.tin[y]);
			y = t.par[t.nxt[y]];
		} else {
			if (ord) p.emplace_back(t.tin[x], t.tin[t.nxt[x]]);
			else p.emplace_back(t.tin[t.nxt[x]], t.tin[x]);
			x = t.par[t.nxt[x]];
		}
	}
	if (t.dst[x] <= t.dst[y] || ord) 
		p.emplace_back(t.tin[x], t.tin[y]);
	else p.emplace_back(t.tin[y], t.tin[x]);
	reverse(rp.begin(), rp.end());
	p.insert(p.begin(), rp.begin(), rp.end());
	return p;
}

// end template //

// Test at https://judge.yosupo.jp/problem/vertex_set_path_composite

const i64 mod = 998244353;

struct Affine {
	i64 a, b;
	i64 operator()(i64 x) { return (a * x + b) % mod; }
};

Affine compose(Affine f, Affine g) {
	return {(f.a * g.a) % mod, f(g.b)};
}

Affine rcompose(Affine f, Affine g) {
	return compose(g, f);
}

Affine e() {
	return {1, 0};
}

template<class S, S (*op)(S, S), S (*e)()>
struct Segtree {
	vector<S> t;
	int n;
	Segtree(int N): t(2 * N, e()), n(N) {
	}
	void set(int i, S val) {
		t[i += n] = val;
		for (i >>= 1; i; i >>= 1) t[i] = op(t[i << 1], t[i << 1 | 1]);
	}
	S sum(int l, int r) {
		S al = e(), ar = e();
		for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
			if (l & 1) al = op(al, t[l++]);
			if (r & 1) ar = op(t[--r], ar);
		}
		return op(al, ar);
	}
};

using segtree = Segtree<Affine, compose, e>;
using segtreerev = Segtree<Affine, rcompose, e>;

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n, q;
	cin >> n >> q;
	vector<Affine> aff(n);
	Tree t(n);
	segtree seg(n);
	segtreerev segr(n);
	for (int i = 0; i < n; i++) {
		int a, b;
		cin >> a >> b;
		aff[i] = {a, b};
	}
	for (int i = 1; i < n; i++) {
		int a, b;
		cin >> a >> b;
		t.addEdge(a, b);
	}
	t.build();
	for (int i = 0; i < n; i++) {
		seg.set(t.tin[i], aff[i]);
		segr.set(t.tin[i], aff[i]);
	}
	for (int k = 0; k < q; k++) {
		int type;
		cin >> type;
		if (type == 0) {
			int p, c, d;
			cin >> p >> c >> d;
			aff[p] = {c, d};
			seg.set(t.tin[p], aff[p]);
			segr.set(t.tin[p], aff[p]);
		} else {
			i64 a, b, x;
			cin >> a >> b >> x;
			auto path = hldPath(t, a, b, true);
			auto f = e();
			for (auto [a, b]: path) {
				if (a > b) {
					f = compose(seg.sum(b, a + 1), f);
				} else {
					f = compose(segr.sum(a, b + 1), f);
				}
			}
			cout << f(x) << endl;
		}
	}
	return 0;
}
