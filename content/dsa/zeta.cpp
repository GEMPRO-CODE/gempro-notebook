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
	int n = sz(a);
	for (int j = 1; j < n; j <<= 1)
		rep (i, 1, n)
			if (i & j) a[i] += a[i ^ j];
	return a;
}
vi subsetMobius(vi a) {
	int n = sz(a);
	for (int j = 1; j < n; j <<= 1)
		rep(i, 1, n)
			if (i & j) a[i] -= a[i ^ j];
	return a;
}
vi supersetZeta(vi a) {
	int n = sz(a);
	for (int j = 1; j < n; j <<= 1)
		rep(i, 1, n)
			if (i & j) a[i ^ j] += a[i];
	return a;
}
vi supersetMobius(vi a) {
	int n = sz(a);
	for (int j = 1; j < n; j <<= 1)
		rep(i, 1, n)
			if (i & j) a[i ^ j] -= a[i];
	return a;
}
vi enumPrimes(int n) {
	vi pr, spf(n);
	rep(i, 2, n) {
		if (spf[i] == 0) {
			spf[i] = i, pr.pb(i);
		}
		for (int j: pr) {
			if (i * j >= n) break;
			spf[i * j] = j;
			if (j == spf[i]) break;
		}
	}
	return pr;
}
vi divZeta(vi a) {
	int n = sz(a);
	for (int p: enumPrimes(n))
		for (int i = 1; i * p < n; i++)
			a[i * p] += a[i];
	return a;
}
vi divMobius(vi a) {
	int n = sz(a);
	for (int p: enumPrimes(n))
		for (int i = n / p; i; i--)
			a[i * p] -= a[i];
	return a;
}
vi mulZeta(vi a) {
	int n = sz(a);
	for (int p: enumPrimes(n))
		for (int i = n / p; i; i--)
			a[i] += a[i * p];
	return a;
}
vi mulMobius(vi a) {
	int n = sz(a);
	for (int p: enumPrimes(n))
		for (int i = 1; i * p < n; i++)
			a[i] -= a[i * p];
	return a;
}
// end template //
