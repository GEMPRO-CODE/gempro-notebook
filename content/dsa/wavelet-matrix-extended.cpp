#include <array>
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>
#include <set>

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
const int H = 30;
using WM = array<vi, H>;
pair<WM, WM> wavMat(vi a) {
	WM wm, pos;
	int n = sz(a);
	vi ord(n), b = a;
	iota(all(ord), 0);
	for (int i = H - 1; i >= 0; i--) {
		wm[i].resize(n + 1), pos[i].resize(n);
		rep(j, 1, n + 1) wm[i][j] = a[j - 1] >> i & 1;
		stable_partition(all(ord), [&](int j) { return ~b[j] >> i & 1; });
		stable_partition(all(a), [&](int x) { return ~x >> i & 1; });
		rep(j, 1, n + 1) wm[i][j] += wm[i][j - 1];
		rep(j, 0, n) pos[i][ord[j]] = j;
	}
	return {wm, pos};
}

tuple<int, int, int, int> getSplit(WM &wm, int h, int l, int r) {
	int a1 = wm[h][l], a0 = l - a1,
		b1 = wm[h][r], b0 = r - b1,
		n = sz(wm[0]) - 1, c = n - wm[h][n];
	return {a0, b0, c + a1, c + b1};
}

void rectUp(WM &wm, int h, int l, int r, int a, auto f) {
	if (h == 0) {
		f(h, l, r);
		return;
	}
	auto [l0, r0, l1, r1] = getSplit(wm, h - 1, l, r);
	if (a >> (h - 1) & 1) rectUp(wm, h - 1, l1, r1, a, f);
	else {
		rectUp(wm, h - 1, l0, r0, a, f);
		f(h - 1, l1, r1);
	}
}

void rectDown(WM &wm, int h, int l, int r, int b, auto f) {
	if (h == 0) return;
	auto [l0, r0, l1, r1] = getSplit(wm, h - 1, l, r);
	if (b >> (h - 1) & 1) {
		f(h - 1, l0, r0);
		rectDown(wm, h - 1, l1, r1, b, f);
	} else rectDown(wm, h - 1, l0, r0, b, f);
}

void rectOp(WM &wm, int l, int r, int a, int b, auto f) {
	for (int h = H; h; h--) {
		auto [l0, r0, l1, r1] = getSplit(wm, h - 1, l, r);
		if ((a ^ b) >> (h - 1) & 1) {
			rectUp(wm, h - 1, l0, r0, a, f);
			rectDown(wm, h - 1, l1, r1, b, f);
			return;
		}
		if (a >> (h - 1) & 1) l = l1, r = r1;
		else l = l0, r = r0;
	}
}
// end template //

void randomTests() {
	int n = 20;
	vi a(n);
	for (int i = 0; i < n; i++) a[i] = uniform_int_distribution<int>(0, 10000)(rng);
	auto [wm, pos] = wavMat(a);
	WM x;
	for (int i = H - 1; i >= 0; i--) {
		x[i].resize(n + 1);
		for (int j = 0; j < n; j++) {
			x[i][pos[i][j] + 1] = a[j];
		}
		for (int j = 1; j <= n; j++) {
			x[i][j] ^= x[i][j - 1];
		}
	}
	int q = 10000;
	while (q--) {
		int l = uniform_int_distribution<int>(0, n)(rng);
		int r = uniform_int_distribution<int>(0, n)(rng);
		int L = uniform_int_distribution<int>(0, 10000)(rng);
		int R = uniform_int_distribution<int>(0, 10000)(rng);
		if (l > r) swap(l, r);
		if (L > R) swap(L, R);
		int ans = 0;
		for (int i = l; i < r; i++) if (L <= a[i] && a[i] < R) {
			ans ^= a[i];
		}
		int out = 0;
		rectOp(wm, l, r, L, R, [&](int h, int il, int ir) { out ^= x[h][ir] ^ x[h][il]; });
		if (out != ans) {
			cout << "Expected: " << ans << endl << "Got: " << out << endl;
			return;
		}
	}
	cout << "PASSED" << endl;
}

void rectangleSum() {
	struct Fenwick {
		vector<ll> t;
		int n;
		Fenwick(int N): t(N), n(N) {}
		void add(int i, ll val) {
			for (i++; i <= n; i += i & -i) t[i - 1] += val;
		}
		ll sum(int r) {
			ll s = 0;
			for (; r >= 0; r -= r & -r) s += t[r - 1];
			return s;
		}
		ll sum(int l, int r) { return sum(r) - sum(l); }
	};
	int n, q;
	cin >> n >> q;
	vector<int> x(n), y(n), w(n), cx, cy;
	for (int i = 0; i < n; i++) cin >> x[i] >> y[i] >> w[i];
	{
		set<int> st(begin(y), end(y));
		cy = vector(begin(st), end(st));
	}
	for (int i = 0; i < n; i++) {
	}
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	rectangleSum();
}
