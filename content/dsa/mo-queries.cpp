#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>

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

// begin template //
vi moOrd(vector<pii> &q, int n) {
	int B = max(1, (int)sqrt(n));
	vi o(sz(q));
	iota(all(o), 0);
	sort(all(o), [&](int i, int j) {
		int a = q[i].fi / B, b = q[j].fi / B;
		if (a != b) return a < b;
		return (a & 1) ? q[i].se > q[j].se : q[i].se < q[j].se;
	});
	return o;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
