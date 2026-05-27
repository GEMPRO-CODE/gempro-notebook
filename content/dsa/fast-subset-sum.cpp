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

// begin template //
vi subSum(vi x) {
	using ull = unsigned long long;
	int s = reduce(all(x));
	vector<ull> bs(s / 64 + 1);
	bs[0] = 1;
	vi p(s + 1, -1);
	rep(i,0,sz(x)) {
		int a = x[i], q = a / 64, r = a % 64;
		for (int j = sz(bs) - 1; j >= 0; --j) {
			ull pre = bs[j], add = 0;
			if (j >= q) add |= (q ? bs[j - q] : pre) << r;
			if (r && j > q) add |= bs[j - q - 1] >> (64 - r);
			bs[j] |= add;
			for (ull k = bs[j] ^ pre; k; k &= k - 1) {
				int t = __builtin_ctzll(k);
				int v = 64 * j + t;
				if (v <= s) p[v] = i;
			}
		}
	}
	return p;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
