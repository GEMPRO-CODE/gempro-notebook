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
const int MOD = 998244353;

ll powm(ll x, ll e) {
	ll r = 1;
	while (e) {
		if (e & 1) (r *= x) %= MOD;
		(x *= x) %= MOD;
		e >>= 1;
	}
	return r;
}

// begin template //
void fwht(vector<ll> &a) {
	for (int k = 1; k < sz(a); k <<= 1)
		for (int i = 0; i < sz(a); i += 2 * k)
			rep (j, i, i + k) {
				ll x = a[j], y = a[j + k];
				a[j] = x + y; // mod: a[j] = (x + y) % MOD;
				a[j + k] = x - y; // mod: a[j + k] = (x - y + MOD) % MOD;
			}
}

vector<ll> xorConv(vector<ll> a, vector<ll> b) {
	int n = 1;
	while (n < max(sz(a), sz(b))) n <<= 1;
	a.resize(n), b.resize(n);
	fwht(a), fwht(b);
	rep (i, 0, n) a[i] *= b[i]; // mod: a[i] = a[i] * b[i] % MOD;
	fwht(a);
	rep (i, 0, n) a[i] /= n; // mod: a[i] = a[i] * invN % MOD;
	return a;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n;
	cin >> n;
	int m = 1 << n;
	vector<ll> a(m), b(m);
	rep(i, 0, m) cin >> a[i];
	rep(i, 0, m) cin >> b[i];
	auto c = xorConv(a, b);
	rep(i, 0, m) cout << c[i] << ' ';
	cout << endl;
}
