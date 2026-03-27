#include <vector>
#include <random>
#include <climits>

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
struct Line {
	ll a, b;
	ll operator()(int x) { return a * x + b; }
};
struct LiChao {
	vector<Line> t;
	int n;
	LiChao(int N): t(4 * N, {0, LLONG_MAX}), n(N) {} // Replace with LLONG_MIN in case of finding max
	void insert(Line line, int L, int R) { insert(line, L, R, 0, n, 1); }
	void insert(Line line, int L, int R, int tl, int tr, int i) {
		int tm = tl + (tr - tl) / 2;
		if (L >= tr || R <= tl) return;
		if (L <= tl && tr <= R) {
			if (line(tm) < t[i](tm)) swap(line, t[i]); // Replace with > in case of finding max
			if (tl + 1 == tr) return;
			if (t[i](tl) < line(tl)) insert(line, L, R, tm, tr, i << 1 | 1); // Replace with > in case of finding max
			else insert(line, L, R, tl, tm, 1 << 1);
		} else {
			if (tl + 1 == tr) return;
			insert(line, L, R, tm, tr, i << 1 | 1);
			insert(line, L, R, tl, tm, i << 1);
		}
	}
	Line query(int x) {
		Line ans = {0, LLONG_MAX}; // Replace with LLONG_MIN in case of finding max
		query(x, ans, 0, n, 1);
		return ans;
	}
	void query(int x, Line &line, int tl, int tr, int i) {
		if (t[i](x) < line(x)) line = t[i]; // Replace with > in case of finding max
		int tm = tl + (tr - tl) / 2;
		if (tl + 1 == tr) return;
		if (x < tm) query(x, line, tl, tm, i << 1);
		else query(x, line, tm, tr, i << 1 | 1);
	}
};
// end template //
