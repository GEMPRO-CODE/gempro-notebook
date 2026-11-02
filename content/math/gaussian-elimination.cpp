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
int gauss(vector<vector<ll>> &a, ll &det) {
	int n = sz(a), m = n ? sz(a[0]) : 0, r = 0;
	det = 1;
	rep(c,0,m) {
		int p = r;
		while (p < n && !a[p][c]) p++;
		if (p == n) continue;
		if (p != r) det = (mod - det) % mod;
		swap(a[p], a[r]);
		ll pivot = (a[r][c] + mod) % mod;
		det = det * pivot % mod;
		ll iv = powm(pivot, mod - 2);
		rep(j,c,m) a[r][j] = a[r][j] * iv % mod;
		rep(i,0,n) if (i != r && a[i][c]) {
			ll x = a[i][c];
			rep(j,c,m) a[i][j] = (a[i][j] - x * a[r][j]) % mod;
		}
		r++;
	}
	rep(i,0,n) rep(j,0,m) if (a[i][j] < 0) a[i][j] += mod;
	if (n != m || r < n) det = 0;
	return r;
}
bool linearSystem(vector<vector<ll>> a, int m,
		vector<ll> &x, vector<vector<ll>> &basis) {
	ll det;
	gauss(a,det);
	vi row(m,-1);
	rep(i,0,sz(a)) {
		int j = 0;
		while (j <= m && !a[i][j]) j++;
		if (j == m) return false;
		if (j < m) row[j] = i;
	}
	x.assign(m,0); basis.clear();
	rep(j,0,m) if (row[j] >= 0) x[j] = a[row[j]][m];
	rep(f,0,m) if (row[f] < 0) {
		vector<ll> v(m); v[f] = 1;
		rep(j,0,m) if (row[j] >= 0)
			v[j] = (mod-a[row[j]][f])%mod;
		basis.pb(v);
	}
	return true;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
