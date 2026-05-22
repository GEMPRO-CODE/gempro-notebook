#pragma GCC target("popcnt")
#include <vector>
#include <iostream>
#include <random>

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

const ll mod = 998244353;

vi subsetZeta(vi a) {
	int n = sz(a);
	for (int j = 1; j < n; j <<= 1)
		rep (i, 1, n)
			if (i & j) {
				a[i] += a[i ^ j];
				if (a[i] >= mod) a[i] -= mod;
			}
	return a;
}

vi subsetMobius(vi a) {
	int n = sz(a);
	for (int j = 1; j < n; j <<= 1)
		rep(i, 1, n)
			if (i & j) {
				a[i] += mod - a[i ^ j];
				if (a[i] >= mod) a[i] -= mod;
			}
	return a;
}

// begin template //
#define T popcount(unsigned(i))
vi subsetConv(vi &a, vi &b) {
	int n = sz(a), m = bit_width(unsigned(n));
	vector ca(m, vi(n)), cb(m, vi(n)), c(m, vi(n));
	rep(i, 0, n) ca[T][i] = a[i], cb[T][i] = b[i];
	rep(i, 0, m) ca[i] = subsetZeta(ca[i]), cb[i] = subsetZeta(cb[i]);
	rep(i, 0, n) rep(j, 0, m) rep(k, 0, m - j) {
		c[j + k][i] += ll(ca[j][i]) * cb[k][i] % mod;
		if (c[j + k][i] >= mod) c[j + k][i] -= mod;
	}
	rep(i, 0, m) c[i] = subsetMobius(c[i]);
	vi r(n);
	rep(i, 0, n) r[i] = c[T][i];
	return r;
}
#undef T
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n;
	cin >> n;
	vector<int> a(1 << n), b(1 << n);
	for (int i = 0; i < 1 << n; i++) cin >> a[i];
	for (int i = 0; i < 1 << n; i++) cin >> b[i];
	auto c = subsetConv(a, b);
	for (int i = 0; i < 1 << n; i++) {
		cout << c[i] << ' ';
	}
	cout << endl;
}
