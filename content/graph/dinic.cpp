#include <vector>
#include <cstdint>
#include <climits>
#include <queue>
#include <iostream>

using namespace std;

using i32 = int32_t;
using i64 = int64_t;
using VI = vector<i32>;
using VVI = vector<VI>;

// begin template //
struct Dinic {
	struct Edge {
		i64 s, t, f, c, r;
	};
	vector<Edge> e;
	VVI adj;
	VI ne, lvl;
	Dinic(int n): adj(n), ne(n), lvl(n) {}
	void addEdge(int a, int b, i64 c) {
		adj[a].push_back(e.size());
		e.emplace_back(a, b, 0, c, e.size() + 1);
		adj[b].push_back(e.size());
		e.emplace_back(b, a, 0, 0, e.size() - 2);
	}
	bool bfs(int s, int t) {
		queue<int> q;
		q.push(s);
		fill(lvl.begin(), lvl.end(), -1);
		lvl[s] = 0;
		while (q.size()) {
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
	i64 dfs(int x, int t, i64 f) {
		if (f == 0) return 0;
		if (x == t) return f;
		for (int &te = ne[x]; te < adj[x].size(); te++) {
			int i = adj[x][te], y = e[i].t;
			if (lvl[y] != lvl[x] + 1) continue;
			if (i64 df = dfs(y, t, min(f, e[i].c - e[i].f))) {
				e[i].f += df;
				e[i ^ 1].f -= df;
				return df;
			}
		}
		return 0;
	}
	i64 flow(int s, int t, i64 mx = LLONG_MAX) {
		i64 mf = 0;
		while (true) {
			if (bfs(s, t)) break;
			fill(ne.begin(), ne.end(), 0);
			while (i64 f = dfs(s, t, mx - mf)) mf += f;
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
