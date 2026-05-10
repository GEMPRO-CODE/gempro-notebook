#include <array>
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

// begin template //
const int H = 31;
using WM = array<vi, H>;
WM wavMat(vector<ll> a) {
	WM wm;
	int n = sz(a);
	for (int i = H - 1; i >= 0; i--) {
		wm[i].resize(n + 1);
		rep(j, 1, n + 1) wm[i][j] = a[j - 1] >> i & 1;
		stable_partition(all(a), [&](ll x) { return ~x >> i & 1; });
		rep(j, 1, n + 1) wm[i][j] += wm[i][j - 1];
	}
	return wm;
}

tuple<int, int, int, int> getSplit(WM &wm, int h, int l, int r) {
	int a1 = wm[h][l], a0 = l - a1,
		b1 = wm[h][r], b0 = r - b1,
		n = sz(wm[0]) - 1, c = n - wm[h][n];
	return {a0, b0, c + a1, c + b1};
}

ll rangeKth(WM &wm, int h, int l, int r, int k) {
	if (h == 0) return 0;
	auto [l0, r0, l1, r1] = getSplit(wm, h - 1, l, r);
	int sl = r0 - l0;
	if (k < sl) return rangeKth(wm, h - 1, l0, r0, k);
	return (1 << (h - 1)) + rangeKth(wm, h - 1, l1, r1, k - sl);
}

int cntLess(WM &wm, int h, int l, int r, ll x) {
	if (h == 0) return 0;
	auto [l0, r0, l1, r1] = getSplit(wm, h - 1, l, r);
	if (x >> (h - 1) & 1)
		return r0 - l0 + cntLess(wm, h - 1, l1, r1, x);
	return cntLess(wm, h - 1, l0, r0, x);
}
// end template //

void testRangeKthSmallest() {
	int n, q;
	cin >> n >> q;
	vector<ll> a(n);
	for (int i = 0; i < n; i++) cin >> a[i];
	WM wm = wavMat(a);
	while (q--) {
		int l, r, k;
		cin >> l >> r >> k;
		cout << rangeKth(wm, H, l, r, k) << endl;
	}
}

void testCountLess() {
	int n = 20, q = 200000;
	vector<ll> a(n);
	int A = 1000;
	for (int i = 0; i < n; i++) a[i] = uniform_int_distribution<ll>(0, A)(rng);
	auto wm = wavMat(a);
	while (q--) {
		int l, r;
		ll x;
		l = uniform_int_distribution<int>(0, n)(rng);
		r = uniform_int_distribution<int>(0, n)(rng);
		x = uniform_int_distribution<ll>(0, A)(rng);
		if (l > r) swap(l, r);
		int cnt = 0;
		for (int i = l; i < r; i++) {
			if (a[i] < x) cnt++;
		}
		int out = cntLess(wm, H, l, r, x);
		if (out != cnt) {
			cout << "Expected " << cnt << endl << "Got " << out << endl;
			cout << " a = ";
			for (int i = 0; i < n; i++) cout << a[i] << ' ';
			cout << endl;
			cout << "l = " << l << " r = " << r << " x = " << x << endl;
			return;
		}
	}
	cout << "PASSED" << endl;
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	testCountLess();
}
