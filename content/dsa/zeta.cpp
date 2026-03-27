#include <vector>
#include <random>

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
vi subsetZeta(vi a) {
	int n = a.size();
	for (int j = 1; j < n; j <<= 1)
		rep (i, 1, n)
			if (i & j) a[i] += a[i ^ j];
	return a;
}
vi subsetMobius(vi a) {
	int n = a.size();
	for (int j = 1; j < n; j <<= 1)
		rep(i, 1, n)
			if (i & j) a[i] -= a[i ^ j];
	return a;
}
vi supersetZeta(vi a) {
	int n = a.size();
	for (int j = 1; j < n; j <<= 1)
		rep(i, 1, n)
			if (i & j) a[i ^ j] += a[i];
	return a;
}
vi supersetMobius(vi a) {
	int n = a.size();
	for (int j = 1; j < n; j <<= 1)
		rep(i, 1, n)
			if (i & j) a[i ^ j] -= a[i];
	return a;
}
// end template //
