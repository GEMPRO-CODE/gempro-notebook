#include <algorithm>
#include <utility>
using namespace std;
using ll = long long;

// begin template //
pair<ll,ll> minMod(ll n, ll m, ll a, ll b) {
	a %= m; b %= m;
	ll A = a, M = m, B = b;
	ll ans = m;
	while (true) {
		if (a > m/2) {
			b = ((__int128)a*(n-1)+b)%m;
			a = m-a;
		}
		ans = min(ans,b);
		ll k = ((__int128)a*(n-1)+b)/m;
		if (!k) break;
		n = k; b = (b-m)%a;
		if (b < 0) b += a;
		ll r = m%a; m = a; a = (m-r)%m;
	}
	ll r = M, s = A, x = 0, y = 1;
	while (s) {
		ll q = r/s, t = r%s;
		r = s; s = t;
		t = x - (__int128)q*y; x = y; y = t;
	}
	ll period = M/r;
	ll i = (__int128)x*((ans-B)/r)%period;
	if (i < 0) i += period;
	return {ans,i};
}
// end template //
