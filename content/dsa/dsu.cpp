#include <vector>
#include <random>
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
#define rep(i,a,b) for (int i = (a); i < (b); ++i)

mt19937 rng(random_device{}());

// begin template //
struct DSU {
	vi par, cnt, hist;
	DSU(int n): par(n), cnt(n, 1) {
		iota(all(par), 0);
	}
	int root(int x) {
		if (par[x] == x) return x;
		return root(par[x]);
	}
	bool merge(int x, int y) {
		x = root(x), y = root(y);
		if (x == y) { hist.pb(-1); return false; }
		if (cnt[x] < cnt[y]) swap(x, y);
		hist.pb(y);
		cnt[x] += cnt[y], par[y] = x;
		return true;
	}
	bool rollback() {
		int y = hist.back(); hist.pop_back();
		if (y < 0) return false;
		cnt[par[y]] -= cnt[y]; par[y] = y;
		return true;
	}
};
// end template //
