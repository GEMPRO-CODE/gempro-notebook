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
#define rep(i,a,b) for (int i = (a); i < (b); i++)

mt19937 rng(random_device{}());

const int mod = 998244353;

ll powm(ll x, ll e) {
	ll r = 1;
	while (e) {
		if (e & 1) r = r * x % mod;
		x = x * x % mod;
		e >>= 1;
	}
	return r;
}

// begin template //
int gauss(vector<vector<ll>> &a) {
	int n = sz(a), m = sz(a[0]), r = 0;
	rep(c,0,m) {
		int p = r;
		while (p < n && !a[p][c]) p++;
		if (p == n) continue;
		swap(a[p], a[r]);
		ll iv = powm((a[r][c] % mod + mod) % mod, mod - 2);
		rep(j,c,m) a[r][j] = a[r][j] * iv % mod;
		rep(i,0,n) if (i != r && a[i][c]) {
			ll x = a[i][c];
			rep(j,c,m) a[i][j] = (a[i][j] - x * a[r][j]) % mod;
		}
		r++;
	}
	rep(i,0,n) rep(j,0,m) if (a[i][j] < 0) a[i][j] += mod;
	return r;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
