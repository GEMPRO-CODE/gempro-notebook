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

const int mod = 998244353;

// begin template //
int linRec(vi s, vi tr, ll k) { // s = first m terms, 0-indexed kth term
	int n = sz(tr);
	if (k < n) return s[k];
	auto mul = [&](vi a, vi b) {
		vi c(2 * n + 1);
		rep (i, 0, n + 1) rep (j, 0, n + 1)
			c[i + j] = (c[i + j] + 1LL * a[i] * b[j]) % mod;
		for (int i = 2 * n; i > n; --i) rep (j, 0, n)
			c[i - 1 - j] = (c[i - 1 - j] + 1LL * c[i] * tr[j]) % mod;
		c.resize(n + 1);
		return c;
	};
	vi pol(n + 1), e(n + 1);
	pol[0] = 1;
	e[1] = 1;
	for (++k; k; k >>= 1) {
		if (k & 1) pol = mul(pol, e);
		e = mul(e, e);
	}
	ll ans = 0;
	rep (i, 0, n) ans = (ans + 1LL * pol[i + 1] * s[i]) % mod;
	return ans;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
	ll n, k;
	cin >> n >> k;
	vi a(n), c(n);
	rep(i, 0, n) cin >> a[i];
	rep(i, 0, n) cin >> c[i];
	cout << linRec(a, c, k) << endl;
}
