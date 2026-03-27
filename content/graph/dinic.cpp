#include <vector>
#include <cstdint>
#include <climits>
#include <queue>
#include <iostream>
#include <random>

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
struct Dinic {
	struct Edge {
		ll s, t, f, c, r;
	};
	vector<Edge> e;
	vector<vi> adj;
	vi ne, lvl;
	Dinic(int n): adj(n), ne(n), lvl(n) {}
	void addEdge(int a, int b, ll c) {
		adj[a].push_back(sz(e));
		e.emplace_back(a, b, 0, c, sz(e) + 1);
		adj[b].push_back(sz(e));
		e.emplace_back(b, a, 0, 0, sz(e) - 2);
	}
	bool bfs(int s, int t) {
		queue<int> q;
		q.push(s);
		fill(all(lvl), -1);
		lvl[s] = 0;
		while (sz(q)) {
			int x = q.front();
			q.pop();
			for (int i: adj[x]) {
				if (e[i].f == e[i].c) continue;
				if (lvl[e[i].t] != -1) continue;
				lvl[e[i].t] = lvl[x] + 1;
				q.push(e[i].t);
			}
		}
		return lvl[t] == -1;
	}
	ll dfs(int x, int t, ll f) {
		if (f == 0) return 0;
		if (x == t) return f;
		for (int &te = ne[x]; te < sz(adj[x]); te++) {
			int i = adj[x][te], y = e[i].t;
			if (lvl[y] != lvl[x] + 1) continue;
			if (ll df = dfs(y, t, min(f, e[i].c - e[i].f))) {
				e[i].f += df;
				e[i ^ 1].f -= df;
				return df;
			}
		}
		return 0;
	}
	ll flow(int s, int t, ll mx = LLONG_MAX) {
		ll mf = 0;
		while (true) {
			if (bfs(s, t)) break;
			fill(all(ne), 0);
			while (ll f = dfs(s, t, mx - mf)) mf += f;
		}
		return mf;
	}
};
// end template //

// Test at https://cses.fi/problemset/task/1694/

int main() {
	int n, m;
	cin >> n >> m;
	Dinic g(n);
	for (int i = 0; i < m; i++) {
		int a, b, c;
		cin >> a >> b >> c;
		a--;b--;
		g.addEdge(a, b, c);
	}
	cout << g.flow(0, n - 1) << endl;
	return 0;
}
