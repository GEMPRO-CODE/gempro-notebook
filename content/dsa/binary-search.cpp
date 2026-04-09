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

// begin template //
void binSearch(ll &lo, ll &hi, auto cond) {
	while (hi - lo > 1) {
		int mid = lo + (hi - lo) / 2;
		if (cond(mid)) hi = mid; else lo = mid;
	}
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
