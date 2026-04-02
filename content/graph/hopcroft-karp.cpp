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
struct HK {
	int n, m;
	vector<vi> g;
	vi l, r, d;
	HK(int n, int m) : n(n), m(m), g(n), l(n, -1), r(m, -1), d(n) {}
	void addEdge(int a, int b) { g[a].pb(b); }
	bool bfs() {
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
	}
	bool dfs(int v) {
		for (int u : g[v]) {
			int w = r[u];
			if (w < 0 || (d[w] == d[v] + 1 && dfs(w))) {
				l[v] = u, r[u] = v;
				return 1;
			}
		}
		return d[v] = -1, 0;
	}
	int maxMatch() {
		int ans = 0;
		while (bfs()) rep(i, 0, n) ans += l[i] < 0 && dfs(i);
		return ans;
	}
};
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
