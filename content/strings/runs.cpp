#include <vector>
#include <random>
#include <string>
#include <iostream>
#include <set>
#include <algorithm>

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

vi zFunc(string s) {
	int n = sz(s), l = 0, r = 0;
	vi z(n);
	if (!n) return z;
	rep(i, 1, n) {
		if (i < r) z[i] = min(r - i, z[i - l]);
		while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
		if (i + z[i] > r) l = i, r = i + z[i];
	}
	z[0] = n;
	return z;
}

// begin template //
using tiii = tuple<int, int, int>;
vector<tiii> getRuns(string& S) {
	int N = sz(S);
	vector<vector<pii>> bp(N + 1);
	auto sub = [&](string &l, string &r) {
		vector<tiii> res;
		int n = sz(l), m = sz(r);
		string s = l, t = r;
		t.insert(end(t), all(l));
		t.insert(end(t), all(r));
		reverse(all(s));
		vi ZS = zFunc(s), ZT = zFunc(t);
		rep(p, 1, n + 1) {
			int a = p == n ? p : min(ZS[p] + p, n);
			int b = min(ZT[n + m - p], m);
			if (a + b < 2 * p) continue;
			res.eb(p, a, b);
		}
		return res;
	};
	auto dfs = [&](auto &&self, int L, int R) -> void {
		if (R - L <= 1) return;
		int M = (L + R) / 2;
		self(self, L, M), self(self, M, R);
		string SL{begin(S) + L, begin(S) + M};
		string SR{begin(S) + M, begin(S) + R};
		auto s1 = sub(SL, SR);
		for (auto &[p, a, b] : s1) bp[p].eb(M - a, M + b);
		reverse(all(SL));
		reverse(all(SR));
		auto s2 = sub(SR, SL);
		for (auto& [p, a, b] : s2) bp[p].eb(M - b, M + a);
	};
	dfs(dfs, 0, N);
	vector<tiii> res;
	set<pii> done;
	rep(p, 0, N + 1) {
		auto& LR = bp[p];
		sort(all(LR), [](pii x, pii y) {
			if (x.fi == y.fi) return x.se > y.se;
			return x.fi < y.fi;
		});
		int r = -1;
		for (auto &lr : LR) {
			if (r >= lr.se) continue;
			r = lr.se;
			if (!done.count(lr)) {
				done.insert(lr);
				res.eb(p, lr.fi, lr.se);
			}
		}
	}
	return res;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
	string s;
	cin >> s;
	vector<tiii> runs = getRuns(s);
	cout << sz(runs) << endl;
	for (auto [a, b, c]: runs) {
		cout << a << ' ' << b << ' ' << c << endl;
	}
}
