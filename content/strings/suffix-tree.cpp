#include <vector>
#include <random>
#include <utility>
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
struct ST {
	struct N {
		int l, r, p, suf;
		vi ch;
	};
	vector<N> t;
	vi dep;
	int nn(int l, int r, int p, int suf = -1) {
		t.pb({l, r, p, suf, {}});
		dep.pb(p == -1 ? 0 : dep[p] + r - l);
		if (p != -1) t[p].ch.pb(sz(t) - 1);
		return sz(t) - 1;
	}
	ST(string& s, vi& sa, vi& lcp) {
		int n = sz(s), rt = nn(0, 0, -1), last = rt;
		rep(i,0,n) {
			int x = sa[i], k = i ? lcp[i] : 0;
			while (dep[last] > k) last = t[last].p;
			if (dep[last] < k) {
				int y = t[last].ch.back(), z = nn(t[y].l, t[y].l + k - dep[last], last);
				t[last].ch.back() = z;
				t[y].p = z;
				t[y].l += k - dep[last];
				t[z].ch.pb(y);
				dep[z] = k;
				last = z;
			}
			last = nn(x + k, n, last, x);
		}
	}
};
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
