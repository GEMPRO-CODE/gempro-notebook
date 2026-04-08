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
		if (e & 1) (r *= x) %= mod;
		(x *= x) %= mod;
		e >>= 1;
	}
	return r;
}

// begin template //
ll lagrange(const vector<ll> &xs, const vector<ll> &ys, ll x) {
	int n = sz(xs); ll ans = 0;
	rep(i,0,n) if (xs[i] == x) return ys[i];
	rep(i,0,n) {
		ll a = ys[i], b = 1;
		rep(j,0,n) if (i != j) a = a * (x - xs[j] + mod) % mod, b = b * (xs[i] - xs[j] + mod) % mod;
		ans = (ans + a * powm(b, mod - 2)) % mod;
	}
	return ans;
}

ll lagrange(const vector<ll> &y, ll x) {
	int n = sz(y);
	x = (x % mod + mod) % mod;
	if (x < n) return y[x];
	vector<ll> p(n + 1, 1), s(n + 1, 1), a(n);
	rep(i,0,n) p[i + 1] = p[i] * (x - i + mod) % mod;
	for (int i = n - 1; i >= 0; --i) s[i] = s[i + 1] * (x - i + mod) % mod;
	a[0] = 1;
	rep(i,1,n) a[i] = a[i - 1] * i % mod;
	a[n - 1] = powm(a[n - 1], mod - 2);
	for (int i = n - 1; i; --i) a[i - 1] = a[i] * i % mod;
	ll ans = 0;
	rep(i,0,n) ans = (ans + p[i] * s[i + 1] % mod * a[i] % mod * a[n - 1 - i] % mod * (((n - 1 - i) & 1) ? mod - y[i] : y[i])) % mod;
	return ans;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
