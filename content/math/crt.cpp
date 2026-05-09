#include <vector>
#include <utility>
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

ll egcd(ll a, ll b, ll &x, ll &y) {
	if (b == 0) {
		x = 1, y = 0;
		return a;
	}
	ll g = egcd(b, a % b, y, x);
	y -= (a / b) * x;
	return g;
}

// begin template //
pair<ll, ll> crt(ll r1, ll m1, ll r2, ll m2) {
	ll x, y, g = egcd(m1, m2, x, y);
	if ((r2 - r1) % g) return {0, -1};
	ll k = m2 / g, m = m1 / g * m2;
	ll q = ((r2 - r1) / g % k + k) % k;
	x = ((x % k) + k) % k;
	ll r = r1 + (__int128)m1 * (q * x % k);
	return {(r % m + m) % m, m};
}
// end template //

// Test at https://judge.yosupo.jp/problem/garner

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n; cin >> n;
	ll r = 0, m = 1;
	for (int i = 0; i < n; i++) {
		ll ri, mi; cin >> ri >> mi;
		auto [nr, nm] = crt(r, m, ri, mi);
		if (nm == -1) { cout << -1 << "\n"; return 0; }
		r = nr; m = nm;
	}
	cout << r << "\n";
}
