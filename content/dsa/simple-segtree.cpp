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
struct Segtree {
	using S = int;
	const S e = 0;
	S op(S a, S b) { return max(a, b); }
	vector<S> t;
	int n;
	Segtree(int N): t(2 * N, e), n(N) {}
	void set(int i, S val) {
		t[i += n] = val;
		while (i >>= 1) t[i] = op(t[i << 1], t[i << 1 | 1]);
	}
	S query(int l, int r) {
		S al = e, ar = e;
		for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
			if (l & 1) al = op(al, t[l++]);
			if (r & 1) ar = op(t[--r], ar);
		}
		return op(al, ar);
	}
};
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
