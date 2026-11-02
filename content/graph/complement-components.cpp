#include <vector>
#include <numeric>
#include <utility>
using namespace std;
using vi = vector<int>;

// begin template //
vector<vi> complementCC(vector<vi> &g) {
	int n = g.size();
	vi nxt(n+1), mark(n, -1);
	iota(nxt.begin(),nxt.end(),1); nxt[n] = 0;
	vector<vi> comps;
	while (nxt[n] != n) {
		int s = nxt[n]; nxt[n] = nxt[s];
		vi q{s};
		for (int i = 0; i < int(q.size()); ++i) {
			int u = q[i];
			for (int v : g[u]) mark[v] = u;
			for (int p = n; nxt[p] != n;) {
				int v = nxt[p];
				if (mark[v] == u) p = v;
				else nxt[p] = nxt[v], q.push_back(v);
			}
		}
		comps.push_back(move(q));
	}
	return comps;
}
// end template //
