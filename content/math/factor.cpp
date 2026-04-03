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

// begin template //
mt19937_64 rng64(random_device{}());

using ull = unsigned long long;
using u128 = __uint128_t;

ull modMul(ull a, ull b, ull mod) { return (u128)a * b % mod; }

ull rho(ull n) {
	if (n % 2 == 0) return 2;
	if (n % 3 == 0) return 3;
	ull c = uniform_int_distribution<ull>(1, n - 1)(rng);
	ull x = uniform_int_distribution<ull>(0, n - 1)(rng), y = x, d = 1;
	auto f = [&](ull x) { return (modMul(x, x, n) + c) % n; };
	while (d == 1) {
		x = f(x);
		y = f(f(y));
		d = gcd(x > y ? x - y : y - x, n);
	}
	return d == n ? rho(n) : d;
}

void factor(ull n, vector<ull> &fs) {
	if (n == 1) return;
	if (isPrime(n)) { fs.pb(n); return; }
	ull d = rho(n);
	factor(d, fs);
	factor(n / d, fs);
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n;
	cin >> n;
	while (n--) {
		ll a;
		cin >> a;
		vector<ull> d;
		factor(a, d);
		sort(all(d));
		cout << sz(d) << ' ';
		for (ll i: d) {
			cout << i << ' ';
		}
		cout << endl;
	}
}
