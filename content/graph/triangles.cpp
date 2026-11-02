#include <vector>
#include <utility>
using namespace std;
using vi = vector<int>;

// begin template //
void triangles(vector<vi> &g, auto emit) {
	int n = g.size();
	vector<vi> out(n);
	vi mark(n, -1);
	for (int u = 0; u < n; ++u)
		for (int v : g[u])
			if (pair{g[u].size(),u} < pair{g[v].size(),v})
				out[u].push_back(v);
	for (int u = 0; u < n; ++u) {
		for (int v : out[u]) mark[v] = u;
		for (int v : out[u]) for (int w : out[v])
			if (mark[w] == u) emit(u,v,w);
	}
}
// end template //
