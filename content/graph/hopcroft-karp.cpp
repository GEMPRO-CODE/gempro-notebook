#include <vector>
#include <random>
#include <iostream>
#include <queue>

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
int hopKarp(vector<vi> &g, vi &l, vi &r) {
	int n = sz(l), m = sz(r);
	vi d(n);
	auto bfs = [&]() {
		queue<int> q;
		rep(i, 0, n) d[i] = l[i] < 0 ? q.push(i), 0 : -1;
		bool f = 0;
		while (sz(q)) {
			int v = q.front(); q.pop();
			for (int u : g[v]) {
				int w = r[u];
				if (w < 0) f = 1;
				else if (d[w] < 0) d[w] = d[v] + 1, q.push(w);
			}
		}
		return f;
	};
	auto dfs = [&](auto &&self, int v) -> bool {
		for (int u : g[v]) {
			int w = r[u];
			if (w < 0 || (d[w] == d[v] + 1 && self(self, w))) {
				l[v] = u, r[u] = v;
				return 1;
			}
		}
		return d[v] = -1, 0;
	};
	int ans = 0;
	while (bfs()) rep(i, 0, n) ans += l[i] < 0 && dfs(dfs, i);
	return ans;
}
// end template //

// Test at: https://judge.yosupo.jp/problem/bipartitematching

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n, m, e;
	cin >> n >> m >> e;
	vector<vi> g(n);
	vi l(n, -1), r(m, -1);
	rep(i, 0, e) {
		int a, b;
		cin >> a >> b;
		g[a].pb(b);
	}
	int k = hopKarp(g, l, r);
	cout << k << endl;
	rep(i, 0, n) {
		if (l[i] != -1) cout << i << ' ' << l[i] << endl;
	}
}
