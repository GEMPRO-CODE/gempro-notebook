#include <array>
#include <iostream>
#include <random>
#include <vector>

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

const int MOD = 998244353, ROOT = 62;

ll powm(ll x, ll e) {
	ll r = 1;
	while (e) {
		if (e & 1) (r *= x) %= MOD;
		e >>= 1;
		(x *= x) %= MOD;
	}
	return r;
}

// begin template //
// const int MOD = 998244353, ROOT = 62;
void ntt(vector<ll> &a) {
	int n = sz(a);
	static array<ll, 1 << 23> r = {1, 1};
	for (static int t = 2, s = 2; t < n; t <<= 1, s++) {
		array z = {1ll, powm(ROOT, MOD >> s)};
		for (int i = t; i < t << 1; i++) r[i] = r[i >> 1] * z[i & 1] % MOD;
	}
	static array<ll, 1 << 23> b, c;
	copy(all(a), b.begin());
	for (int m = n; m >>= 1;) {
		for (int l = 0; l < n; l += m << 1) 
			rep (i, 0, m) 
				c[l + i] = b[l + 2 * i], c[l + m + i] = b[l + 2 * i + 1];
		copy(c.begin(), c.begin() + n, b.begin());
	}
	for (int m = 1; m < n; m <<= 1) {
		for (int l = 0; l < n; l += m << 1)
			rep (i, 0, m) {
				ll z = r[m + i] * b[l + m + i] % MOD;
				c[l + i] = b[l + i] + z;
				c[l + m + i] = b[l + i] + (MOD - z);
			}
		rep (i, 0, n) b[i] = c[i] < MOD ? c[i] : c[i] - MOD;
	}
	rep(i, 0, n) a[i] = b[i];
}

vector<ll> conv(vector<ll> &a, vector<ll> &b) {
	if (a.empty() || b.empty()) return {};
	int s = sz(a) + sz(b) - 1, n = 1;
	while (n < s) n <<= 1;
	int inv = powm(n, MOD - 2);
	vector<ll> L(a), R(b), out(n);
	L.resize(n), R.resize(n);
	ntt(L), ntt(R);
	rep(i, 0, n) out[-i & (n - 1)] = L[i] * R[i] % MOD * inv % MOD;
	ntt(out);
	return {out.begin(), out.begin() + s};
}
// end template //

// Test at https://judge.yosupo.jp/problem/convolution_mod

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n, m;
	cin >> n >> m;
	vector<ll> a(n), b(m);
	for (int i = 0; i < n; i++) {
		cin >> a[i];
	}
	for (int i = 0; i < m; i++) {
		cin >> b[i];
	}
	auto c = conv(a, b);
	for (int x: c) {
		cout << x << ' ';
	}
	cout << endl;
	return 0;
}
