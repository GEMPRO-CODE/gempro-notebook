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

ll mod = 998244353;

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
int sqrtMod(int a) { // mod prime
	if (a == 0 || mod == 2) return a;
	if (powm(a, (mod - 1) / 2) != 1) return -1;
	if (mod % 4 == 3) return powm(a, (mod + 1) / 4);
	int s = __builtin_ctz(mod - 1), q = (mod - 1) >> s, z = 2;
	while (powm(z, (mod - 1) / 2) != mod - 1) z++;
	ll c = powm(z, q), x = powm(a, (q + 1) / 2), t = powm(a, q);
	for (int m = s; t != 1; ) {
		ll tt = t;
		int i = 0;
		while (tt != 1) tt = tt * tt % mod, i++;
		ll b = powm(c, 1 << (m - i - 1));
		x = x * b % mod;
		c = b * b % mod;
		t = t * c % mod;
		m = i;
	}
	return min<ll>(x, mod - x);
}
// end template //

// Test at: https://judge.yosupo.jp/problem/sqrt_mod

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int t;
	cin >> t;
	while (t--) {
		int a;
		cin >> a >> mod;
		cout << sqrtMod(a) << endl;
	}
}
