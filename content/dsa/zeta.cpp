#include <utility>
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
vi subsetZeta(vi a, bool inv = false) {
	for (int j = 1; j < sz(a); j <<= 1)
		rep(i,1,sz(a)) if (i & j) inv ? a[i] -= a[i^j] : a[i] += a[i^j];
	return a;
}
vi supersetZeta(vi a, bool inv = false) {
	for (int j = 1; j < sz(a); j <<= 1)
		rep(i,1,sz(a)) if (i & j) inv ? a[i^j] -= a[i] : a[i^j] += a[i];
	return a;
}
vi enumPrimes(int n) {
	vi pr, spf(n);
	rep(i, 2, n) {
		if (spf[i] == 0) spf[i] = i, pr.pb(i);
		for (int j: pr) {
			if (i * j >= n) break;
			spf[i * j] = j;
			if (j == spf[i]) break;
		}
	}
	return pr;
}
vi divZeta(vi a, bool inv = false) {
	int n = sz(a)-1;
	for (int p : enumPrimes(n+1)) rep(j,1,n/p+1) {
		int i = inv ? n/p+1-j : j;
		inv ? a[i*p] -= a[i] : a[i*p] += a[i];
	}
	return a;
}
vi mulZeta(vi a, bool inv = false) {
	int n = sz(a)-1;
	for (int p : enumPrimes(n+1)) rep(j,1,n/p+1) {
		int i = inv ? j : n/p+1-j;
		inv ? a[i] -= a[i*p] : a[i] += a[i*p];
	}
	return a;
}
// end template //
