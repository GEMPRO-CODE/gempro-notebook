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
template <class S, S (*op)(S, S)> struct SparseTable {
	vector<vector<S>> t;
	vi lg;
	void build(vector<S> &v) {
		int n = ssize(v);
		lg.assign(n + 1, 0);
		rep (i, 2, n + 1) lg[i] = lg[i >> 1] + 1;
		t.assign(lg[n] + 1, vector<S>(n));
		t[0] = v;
		rep (k, 1, lg[n] + 1)
			for (int i = 0; i + (1 << k) <= n; i++)
				t[k][i] = op(t[k - 1][i], t[k - 1][i + (1 << (k - 1))]);
	}
	S query(int l, int r) {
		int k = lg[r - l];
		return op(t[k][l], t[k][r - (1 << k)]);
	}
};
// end template //

// Test at: https://judge.yosupo.jp/problem/staticrmq

using RMQ = SparseTable<int, [](int a, int b) { return min(a, b); }>;

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n, q;
	cin >> n >> q;
	vi a(n);
	rep(i, 0, n) cin >> a[i];
	RMQ rmq;
	rmq.build(a);
	while (q--) {
		int l, r;
		cin >> l >> r;
		cout << rmq.query(l, r) << endl;
	}
}
