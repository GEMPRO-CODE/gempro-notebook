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
	LiChao(int N): t(4 * N, {0, LLONG_MAX}), n(N) {}
	void insert(Line line) { insert(line, 0, n); }
	void insert(Line line, int L, int R) { insert(line, L, R, 0, n, 1); }
	void insert(Line line, int L, int R, int tl, int tr, int i) {
		int tm = tl + (tr - tl) / 2;
		if (L >= tr || R <= tl) return;
		if (L <= tl && tr <= R) {
			if (line(tm) < t[i](tm)) swap(line, t[i]);
			if (tl + 1 == tr) return;
			if (t[i](tl) < line(tl)) insert(line, L, R, tm, tr, i << 1 | 1);
			else insert(line, L, R, tl, tm, i << 1);
		} else {
			if (tl + 1 == tr) return;
			insert(line, L, R, tm, tr, i << 1 | 1);
			insert(line, L, R, tl, tm, i << 1);
		}
	}
	Line query(int x) {
		Line line = {0, LLONG_MAX};
		for (int tl = 0, tr = n, i = 1;;) {
			if (t[i](x) < line(x)) line = t[i];
			if (tl + 1 == tr) return line;
			int tm = tl + (tr-tl)/2;
			if (x < tm) tr = tm, i <<= 1;
			else tl = tm, i = i << 1 | 1;
		}
	}
};
// end template //
