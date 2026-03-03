#include <vector>
#include <queue>
#include <climits>
#include <cstdint>

using namespace std;

using i32 = int32_t;
using i64 = int64_t;
using VI = vector<i32>;
using VVI = vector<VI>;

// begin template //
struct MCMF {
	struct Edge { i32 s, t; i64 f, c, w; };
	vector<Edge> e;
	VVI adj;
	vector<i64> dist;
	VI par;
	vector<bool> inq;
	MCMF(int n): adj(n), dist(n), par(n), inq(n) {}
	void addEdge(int a, int b, i64 c, i64 w) {
		adj[a].push_back(e.size());
		e.push_back({a, b, 0, c, w});
		adj[b].push_back(e.size());
		e.push_back({b, a, 0, 0, -w});
	}
	bool spfa(int s, int t) {
		fill(dist.begin(), dist.end(), LLONG_MAX);
		fill(inq.begin(), inq.end(), false);
		queue<int> q;
		dist[s] = 0; inq[s] = true; q.push(s);
		while (!q.empty()) {
			int x = q.front(); q.pop(); inq[x] = false;
			for (int i: adj[x]) {
				auto &ei = e[i];
				if (ei.f == ei.c || dist[x] + ei.w >= dist[ei.t]) continue;
				dist[ei.t] = dist[x] + ei.w;
				par[ei.t] = i;
				if (!inq[ei.t]) { inq[ei.t] = true; q.push(ei.t); }
			}
		}
		return dist[t] != LLONG_MAX;
	}
	// returns {max_flow, min_cost}
	pair<i64, i64> flow(int s, int t) {
		i64 mf = 0, mc = 0;
		while (spfa(s, t)) {
			i64 f = LLONG_MAX;
			for (int x = t; x != s; ) { int i = par[x]; f = min(f, e[i].c - e[i].f); x = e[i].s; }
			for (int x = t; x != s; ) { int i = par[x]; e[i].f += f; e[i^1].f -= f; x = e[i].s; }
			mf += f; mc += f * dist[t];
		}
		return {mf, mc};
	}
};
// end template //

// Test at https://cses.fi/problemset/task/2195/

#include <iostream>
int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n; cin >> n;
	MCMF g(2 * n + 2);
	int s = 2 * n, t = 2 * n + 1;
	for (int i = 0; i < n; i++) g.addEdge(s, i, 1, 0);
	for (int i = 0; i < n; i++) g.addEdge(n + i, t, 1, 0);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			int c; cin >> c;
			g.addEdge(i, n + j, 1, c);
		}
	auto [f, c] = g.flow(s, t);
	cout << c << "\n";
}
