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
template<class S, S (*op)(S, S), S (*e)(), class U, S (*mapping)(U, S),
         U (*compose)(U, U), U (*id)()>
struct LazySegtree {
	vector<S> t;
	vector<U> d;
	int n;
	LazySegtree(int N): t(4 * N, e()), d(4 * N, id()), n(N) {}
	void applyNode(int i, U u) {
		t[i] = mapping(u, t[i]);
		d[i] = compose(u, d[i]);
	}
	void push(int i) {
		applyNode(i << 1, d[i]);
		applyNode(i << 1 | 1, d[i]);
		d[i] = id();
	}
	void pull(int i) { t[i] = op(t[i << 1], t[i << 1 | 1]); }
	void set(int j, S val) { set(j, val, 0, n, 1); }
	void set(int j, S val, int tl, int tr, int i) {
		if (tl + 1 == tr) {
			t[i] = val;
			d[i] = id();
			return;
		}
		push(i);
		int tm = (tl + tr) / 2;
		if (j < tm) set(j, val, tl, tm, i << 1);
		else set(j, val, tm, tr, i << 1 | 1);
		pull(i);
	}
	void apply(int l, int r, U u) { apply(l, r, u, 0, n, 1); }
	void apply(int l, int r, U u, int tl, int tr, int i) {
		if (r <= tl || tr <= l) return;
		if (l <= tl && tr <= r) return applyNode(i, u);
		push(i);
		int tm = (tl + tr) / 2;
		apply(l, r, u, tl, tm, i << 1);
		apply(l, r, u, tm, tr, i << 1 | 1);
		pull(i);
	}
	S sum(int l, int r) { return sum(l, r, 0, n, 1); }
	S sum(int l, int r, int tl, int tr, int i) {
		if (r <= tl || tr <= l) return e();
		if (l <= tl && tr <= r) return t[i];
		push(i);
		int tm = (tl + tr) / 2;
		return op(sum(l, r, tl, tm, i << 1),
		          sum(l, r, tm, tr, i << 1 | 1));
	}
};
// end template //

// Test at https://judge.yosupo.jp/problem/range_affine_point_get

using i64 = long long;
const i64 mod = 998244353;

struct Upd {
	i64 a, b;
};

i64 mapping(Upd f, i64 x) {
	return (f.a * x + f.b) % mod;
}

Upd compose(Upd f, Upd g) {
	return {f.a * g.a % mod, (f.a * g.b + f.b) % mod};
}

Upd id() {
	return {1, 0};
}

using segtree = LazySegtree<i64, [](i64 a, i64 b) { return a == -1 ? b : a; }, []() { return -1ll; }, Upd, mapping, compose, id>;

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n, q;
	cin >> n >> q;
	segtree seg(n);
	for (int i = 0; i < n; i++) {
		int a;
		cin >> a;
		seg.set(i, a);
	}
	while (q--) {
		int t;
		cin >> t;
		if (t == 0) {
			int l, r, b, c;
			cin >> l >> r >> b >> c;
			seg.apply(l, r, {b, c});
		} else {
			int i;
			cin >> i;
			cout << seg.sum(i, i + 1) << endl;
		}
	}
	return 0;
}
