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

const int mod = 998244353;

ll powm(ll x, ll e) {
	ll r = 1;
	while (e) {
		if (e & 1) r = r * x % mod;
		e >>= 1;
		x = x * x % mod;
	}
	return r;
}

// begin template //
vi berlekampMassey(vi s) {
	vi c(1, 1), b(1, 1);
	int l = 0, m = 1; ll bb = 1;
	rep (n, 0, sz(s)) {
		ll d = 0;
		rep (i, 0, l + 1) d = (d + 1LL * c[i] * s[n - i]) % mod;
		if (d < 0) d += mod;
		if (!d) { m++; continue; }
		vi t = c;
		ll x = d * powm(bb, mod - 2) % mod;
		if (sz(c) < sz(b) + m) c.resize(sz(b) + m);
		rep (i, 0, sz(b)) c[i + m] = (c[i + m] - x * b[i]) % mod;
		if (2 * l <= n) l = n + 1 - l, b = t, bb = d, m = 1;
		else m++;
	}
	c.erase(begin(c));
	for (int &x : c) x = (mod - x) % mod;
	return c;
}
// end template //

// Test at: https://judge.yosupo.jp/problem/find_linear_recurrence

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n;
	cin >> n;
	vi s(n);
	rep(i, 0, n) cin >> s[i];
	vi c = berlekampMassey(s);
	cout << sz(c) << endl;
	for (int x: c) cout << x << ' ';
	cout << endl;
}
