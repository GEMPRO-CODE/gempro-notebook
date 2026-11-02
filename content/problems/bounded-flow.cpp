#define main bounded_flow_dinic_main
#include "../graph/dinic.cpp"
#undef main

// begin template //
struct BoundedFlow {
	int n;
	Dinic d;
	vector<ll> bal, lo;
	vi ids;
	BoundedFlow(int n): n(n), d(n+2), bal(n) {}
	int add(int u, int v, ll l, ll r) {
		ids.push_back(d.e.size()); lo.push_back(l);
		d.addEdge(u, v, r-l);
		bal[u] -= l; bal[v] += l;
		return ids.size()-1;
	}
	bool feasible() {
		ll need = 0;
		for (int u = 0; u < n; ++u)
			if (bal[u] > 0) d.addEdge(n, u, bal[u]), need += bal[u];
			else d.addEdge(u, n+1, -bal[u]);
		return d.flow(n, n+1, need) == need;
	}
	ll get(int id) { return lo[id]+d.e[ids[id]].f; }
};
// end template //
