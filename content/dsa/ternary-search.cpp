#include <vector>
#include <iostream>
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
int ternarySearch(ll l, ll r, auto f) {
	while (r - l > 2) {
		ll m1 = l + (r - l) / 3, m2 = l + (r - l) / 3 * 2;
		if (f(m1) < f(m2)) l = m1;
		else r = m2;
	}
	int mx = l;
	rep(i, l + 1, r + 1) if (f(i) > f(mx)) mx = i;
	return mx;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
