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
using i128 = __int128_t;

i128 floorSumSq(ll n, ll m, ll a, ll b);

i128 floorSum(ll n, ll m, ll a, ll b) {
	if (!n) return 0;
	if (a >= m || b >= m) {
		ll qa = a / m, qb = b / m;
		return (i128)qa * n * (n - 1) / 2 + (i128)qb * n
			+ floorSum(n, m, a % m, b % m);
	}
	ll k = ((i128)a * (n - 1) + b) / m;
	if (!k) return 0;
	return (i128)k * (n - 1) - floorSum(k, a, m, m - b - 1);
}

i128 floorSumI(ll n, ll m, ll a, ll b) {
	if (!n) return 0;
	i128 s1 = (i128)n * (n - 1) / 2, s2 = (i128)n * (n - 1) * (2 * n - 1) / 6;
	if (a >= m || b >= m) {
		ll qa = a / m, qb = b / m;
		return (i128)qa * s2 + (i128)qb * s1
			+ floorSumI(n, m, a % m, b % m);
	}
	ll k = ((i128)a * (n - 1) + b) / m;
	if (!k) return 0;
	return (i128)k * s1
		- (floorSumSq(k, a, m, m - b - 1) + floorSum(k, a, m, m - b - 1)) / 2;
}
i128 floorSumSq(ll n, ll m, ll a, ll b) {
	if (!n) return 0;
	i128 s1 = (i128)n * (n - 1) / 2, s2 = (i128)n * (n - 1) * (2 * n - 1) / 6;
	if (a >= m || b >= m) {
		ll qa = a / m, qb = b / m;
		return (i128)qa * qa * s2 + 2 * (i128)qa * qb * s1 + (i128)qb * qb * n
			+ 2 * (i128)qa * floorSumI(n, m, a % m, b % m)
			+ 2 * (i128)qb * floorSum(n, m, a % m, b % m)
			+ floorSumSq(n, m, a % m, b % m);
	}
	ll k = ((i128)a * (n - 1) + b) / m;
	if (!k) return 0;
	return (i128)k * k * (n - 1)
		- 2 * floorSumI(k, a, m, m - b - 1)
		- floorSum(k, a, m, m - b - 1);
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
