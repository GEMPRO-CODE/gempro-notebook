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

// begin template //
int rankMod(vector<vi> &a) {
	int n = sz(a), m = sz(a[0]), r = 0;
	rep(c,0,m) {
		int p = r;
		while (p < n && !a[p][c]) p++;
		if (p == n) continue;
		swap(a[p], a[r]);
		int iv = powm(a[r][c], mod - 2);
		rep(i,r+1,n) if (a[i][c]) {
			int f = (ll)a[i][c] * iv % mod;
			rep(j,c,m) {
				a[i][j] = (a[i][j] - (ll)f * a[r][j]) % mod;
				if (a[i][j] < 0) a[i][j] += mod;
			}
		}
		r++;
	}
	return r;
}

int tutteMatchSize(int n, vector<pii> &es, int its = 2) {
	uniform_int_distribution<int> dist(1, mod - 1);
	int ans = 0;
	vector<vi> a(n, vi(n));
	rep(it,0,its) {
		for (auto [u, v] : es) if (u != v) {
			int x = dist(rng);
			a[u][v] += x;
			if (a[u][v] >= mod) a[u][v] -= mod;
			a[v][u] -= x;
			if (a[v][u] < 0) a[v][u] += mod;
		}
		ans = max(ans, rankMod(a) / 2);
	}
	return ans;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
