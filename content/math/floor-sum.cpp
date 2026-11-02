#include <array>
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
array<i128,3> floorSums(ll n, ll m, ll a, ll b) {
	if (!n) return {0,0,0};
	i128 s1 = (i128)n*(n-1)/2, s2 = (i128)n*(n-1)*(2*(i128)n-1)/6;
	ll qa = a/m, qb = b/m;
	if (qa || qb) {
		auto [x,y,z] = floorSums(n,m,a%m,b%m);
		return {x + qa*s1 + (i128)qb*n, y + qa*s2 + qb*s1,
			z + 2*qa*y + 2*qb*x + (i128)qa*qa*s2
			+ 2*(i128)qa*qb*s1 + (i128)qb*qb*n};
	}
	ll k = ((i128)a*(n-1)+b)/m;
	if (!k) return {0,0,0};
	auto [x,y,z] = floorSums(k,a,m,m-b-1);
	return {(i128)k*(n-1)-x, k*s1-(z+x)/2, (i128)k*k*(n-1)-2*y-x};
}
i128 floorSum(ll n, ll m, ll a, ll b) {
	if (!n) return 0;
	if (a >= m || b >= m)
		return (i128)(a/m)*n*(n-1)/2 + (i128)(b/m)*n
			+ floorSum(n,m,a%m,b%m);
	ll k = ((i128)a*(n-1)+b)/m;
	return k ? (i128)k*(n-1)-floorSum(k,a,m,m-b-1) : 0;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
