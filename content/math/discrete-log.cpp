// https://cp-algorithms.com/algebra/discrete-log.html
#include <cmath>
#include <numeric>
#include <unordered_map>
using namespace std;
using ll = long long;

// begin template //
ll discreteLog(ll a, ll b, ll m) {
	a %= m; b %= m;
	auto mul = [](ll x,ll y,ll m) { return (ll)((__int128)x*y % m); };
	ll k = 1 % m, off = 0;
	for (ll g; (g = gcd(a,m)) > 1; ++off) {
		if (b == k) return off;
		if (b % g) return -1;
		b /= g; m /= g;
		k = mul(k,a/g,m);
	}
	if (b == k) return off;
	ll n = sqrtl(m) + 1, an = 1, cur = b;
	unordered_map<ll,ll> baby;
	for (ll j = 0; j < n; ++j) {
		baby[cur] = j;
		cur = mul(cur,a,m), an = mul(an,a,m);
	}
	cur = k;
	for (ll i = 1; i <= n; ++i) {
		cur = mul(cur,an,m);
		if (baby.count(cur)) return (ll)((__int128)i*n - baby[cur] + off);
	}
	return -1;
}
// end template //
