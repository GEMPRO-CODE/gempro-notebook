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
vector<ll> lagrangePoly(const vector<ll> &xs, const vector<ll> &ys) {
	int n = sz(xs);
	vector<ll> p = {1}, f(n);
	rep(i,0,n) {
		p.pb(0);
		for (int j = i + 1; j; --j) p[j] = (p[j - 1] + mod - p[j] * xs[i] % mod) % mod;
		p[0] = p[0] * (mod - xs[i]) % mod;
	}
	rep(i,0,n) {
		vector<ll> q(n);
		q[n - 1] = p[n];
		for (int j = n - 1; j; --j) q[j - 1] = (p[j] + q[j] * xs[i]) % mod;
		ll den = 1;
		rep(j,0,n) if (i != j) den = den * (xs[i] - xs[j] + mod) % mod;
		ll c = ys[i] * powm(den, mod - 2) % mod;
		rep(j,0,n) f[j] = (f[j] + c * q[j]) % mod;
	}
	return f;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n;
	cin >> n;
	vector<ll> x(n), y(n);
	rep(i, 0, n) cin >> x[i];
	rep(i, 0, n) cin >> y[i];
	auto c = lagrangePoly(x, y);
	rep(i, 0, n) {
		cout << c[i] << ' ';
	}
	cout << endl;
}
