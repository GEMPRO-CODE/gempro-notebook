// Adapted from KACTL, Simon Lindholm; CC0.
// https://github.com/kth-competitive-programming/kactl/blob/main/content/graph/EdgeColoring.h
#include <algorithm>
#include <tuple>
#include <utility>
#include <vector>
using namespace std;
using vi = vector<int>;
using pii = pair<int,int>;
#define sz(x) (int)(x).size()
#define all(x) begin(x), end(x)
#define rep(i,a,b) for (int i = (a); i < (b); ++i)

// begin template //
vi edgeColoring(int n, vector<pii> edges) {
	vi cc(n+1), color(sz(edges)), fan(n), free(n), loc;
	for (auto [u,v] : edges) ++cc[u], ++cc[v];
	int u, v, colors = *max_element(all(cc))+1;
	vector<vi> adj(n,vi(colors,-1));
	for (auto edge : edges) {
		tie(u,v) = edge; fan[0] = v;
		loc.assign(colors,0);
		int at = u, end = u, d, c = free[u], ind = 0, i = 0;
		while (d = free[v], !loc[d] && (v = adj[u][d]) != -1)
			loc[d] = ++ind, cc[ind] = d, fan[ind] = v;
		cc[loc[d]] = c;
		for (int cd = d; at != -1; cd ^= c^d, at = adj[at][cd])
			swap(adj[at][cd],adj[end = at][cd^c^d]);
		while (adj[fan[i]][d] != -1) {
			int left = fan[i], right = fan[++i], e = cc[i];
			adj[u][e] = left; adj[left][e] = u;
			adj[right][e] = -1; free[right] = e;
		}
		adj[u][d] = fan[i]; adj[fan[i]][d] = u;
		for (int y : {fan[0],u,end})
			for (int &z = free[y] = 0; adj[y][z] != -1; ++z);
	}
	rep(i,0,sz(edges))
		for (tie(u,v) = edges[i]; adj[u][color[i]] != v; ++color[i]);
	return color;
}
// end template //
