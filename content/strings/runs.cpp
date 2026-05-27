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

vector<tiii> getRuns(string &S) {
	int N = sz(S);
	vector<vector<pii>> B(N + 1);
	auto go = [&](string &l, string &r, int M, int o) {
		int n = sz(l), m = sz(r);
		string s = l, t = r + l + r;
		reverse(all(s));
		auto z = zFunc(s), w = zFunc(t);
		rep(p,1,n+1) {
			int a = p == n ? n : min(p + z[p], n), b = min(w[n + m - p], m);
			if (a + b >= 2 * p) B[p].eb(M - (o ? b : a), M + (o ? a : b));
		}
	};
	auto dfs = [&](auto &&self, int L, int R) -> void {
		if (R - L < 2) return;
		int M = (L + R) >> 1;
		self(self, L, M), self(self, M, R);
		string l(begin(S) + L, begin(S) + M), r(begin(S) + M, begin(S) + R);
		go(l, r, M, 0);
		reverse(all(l)), reverse(all(r));
		go(r, l, M, 1);
	};
	dfs(dfs, 0, N);
	vector<tiii> res;
	set<pii> done;
	rep(p,0,N+1) {
		auto &v = B[p];
		sort(all(v), [](pii a, pii b) { return a.fi == b.fi ? a.se > b.se : a.fi < b.fi; });
		int R = -1;
		for (auto [l, r] : v) if (R < r) {
			R = r;
			if (done.emplace(l, r).se) res.eb(p, l, r);
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
