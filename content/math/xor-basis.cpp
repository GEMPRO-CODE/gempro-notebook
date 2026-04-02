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
struct XorBasis {
	static const int B = 63; // works for nonnegative ll values < 2^63
	ll b[B] = {};
	bool add(ll x) {
		for (int i = B - 1; i >= 0; i--) if (x >> i & 1) {
			if (!b[i]) return b[i] = x, 1;
			x ^= b[i];
		}
		return 0;
	}

	bool canMake(ll x) {
		for (int i = B - 1; i >= 0; i--) if (x >> i & 1) {
			if (!b[i]) return 0;
			x ^= b[i];
		}
		return 1;
	}
	ll maxXor(ll x = 0) {
		for (int i = B - 1; i >= 0; i--) x = max(x, x ^ b[i]);
		return x;
	}
	int rank() {
		int r = 0;
		rep(i, 0, B) r += !!b[i];
		return r;
	}
	void merge(XorBasis &o) {
		rep(i, 0, B) if (o.b[i]) add(o.b[i]);
	}
};
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
