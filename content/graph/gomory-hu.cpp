// Gusfield variant; adapted from KACTL (chilli, Takanori MAEHARA), CC0.
// https://github.com/kth-competitive-programming/kactl/blob/main/content/graph/GomoryHu.h
#include <algorithm>
#include <tuple>
#define main dinic_main
#include "dinic.cpp"
#undef main

// begin template //
vector<tuple<int,int,ll>> gomoryHu(int n, vector<tuple<int,int,ll>> es) {
	vi par(n);
	vector<tuple<int,int,ll>> tree;
	for (int i = 1; i < n; ++i) {
		Dinic g(n);
		for (auto [u,v,c] : es) if (u != v) {
			g.addEdge(u,v,c), g.addEdge(v,u,c);
		}
		ll cut = g.flow(i,par[i]);
		g.bfs(i,par[i]);
		tree.emplace_back(i,par[i],cut);
		for (int j = i + 1; j < n; ++j)
			if (par[j] == par[i] && g.lvl[j] >= 0) par[j] = i;
	}
	return tree;
}
// end template //
