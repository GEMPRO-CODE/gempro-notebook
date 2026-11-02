#include <array>
#include <algorithm>
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

const int mod = 998244353, ROOT = 3;

ll powm(ll x, ll e) {
	ll r = 1;
	while (e) {
		if (e & 1) (r *= x) %= mod;
		e >>= 1;
		(x *= x) %= mod;
	}
	return r;
}

// begin template //
// const int mod = 998244353, ROOT = 3;
// could also be mod = 469762049, ROOT = 3
void ntt(vector<ll> &a) {
	int n = sz(a);
	static vector<ll> r = {1, 1};
	for (int t = sz(r); t < n; t <<= 1) {
		r.resize(t << 1);
		ll z = powm(ROOT, (mod - 1) / (t << 1));
		for (int i = t; i < t << 1; i++)
			r[i] = r[i >> 1] * (i & 1 ? z : 1) % mod;
	}
	for (int i = 1, j = 0; i < n; i++) {
		int bit = n >> 1;
		for (; j & bit; bit >>= 1) j ^= bit;
		j ^= bit;
		if (i < j) swap(a[i], a[j]);
	}
	for (int m = 1; m < n; m <<= 1)
		for (int l = 0; l < n; l += m << 1)
			rep(i, 0, m) {
				ll x = a[l + i], z = r[m + i] * a[l + m + i] % mod;
				a[l + i] = (x + z < mod ? x + z : x + z - mod);
				a[l + m + i] = (x < z ? x - z + mod : x - z);
			}
}

vector<ll> conv(vector<ll> &a, vector<ll> &b) {
	if (a.empty() || b.empty()) return {};
	int s = sz(a) + sz(b) - 1, n = 1;
	while (n < s) n <<= 1;
	int inv = powm(n, mod - 2);
	vector<ll> L(a), R(b), out(n);
	L.resize(n), R.resize(n);
	ntt(L), ntt(R);
	rep(i, 0, n) out[-i & (n - 1)] = L[i] * R[i] % mod * inv % mod;
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
