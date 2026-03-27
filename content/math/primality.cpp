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

// begin template //
using i128 = __int128;
ll powm(ll x, ll e, ll mod) {
	ll r = 1;
	while (e) {
		if (e & 1) r = i128(r)* x % mod;
		e >>= 1;
		x = i128(x) * x % mod;
	}
	return r;
}

bool checkPrime(ll p, ll a) {
	ll k = p - 1, d = 1;
	while (~k & 1) k >>= 1;
	for (ll e = k; e; e >>= 1) {
		if (e & 1) d = i128(d) * a % p;
		a = i128(a) * a % p;
	}
	if (d == 1 || d == p - 1) return true;
	while ((k <<= 1) < p - 1) {
		d = i128(d) * d % p;
		if (d == p - 1) return true;
	}
	return false;
}

bool isPrime(ll p) {
	if (p == 1) return false;
	for (int i: {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
		if (p == i) return true;
		if (!checkPrime(p, i)) return false;
	}
	return true;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int q;
	cin >> q;
	while (q--) {
		ll n;
		cin >> n;
		if (isPrime(n)) {
			cout << "Yes" << endl;
		} else {
			cout << "No" << endl;
		}
	}
	return 0;
}
