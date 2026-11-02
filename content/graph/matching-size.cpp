#include <iostream>
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

// begin template //
int matchingSize(int n, vector<pii> &es, int its = 2) {
	uniform_int_distribution<int> dist(1, mod - 1);
	int ans = 0;
	rep(it,0,its) {
		vector<vector<ll>> a(n, vector<ll>(n));
		for (auto [u, v] : es) if (u != v) {
			int x = dist(rng);
			a[u][v] += x;
			if (a[u][v] >= mod) a[u][v] -= mod;
			a[v][u] -= x;
			if (a[v][u] < 0) a[v][u] += mod;
		}
		ll det;
		ans = max(ans, gauss(a, det) / 2);
	}
	return ans;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
