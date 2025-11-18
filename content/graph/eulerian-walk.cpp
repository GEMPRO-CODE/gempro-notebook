#include <vector>
#include <utility>

using namespace std;

using PII = pair<int, int>;
using VI = vector<int>;
using VII = vector<PII>;
using VVII = vector<vector<pair<int, int>>>;

// begin template //
pair<VI, VI> eulerWalk(int n, int m, VVII &adj, int s = 0) {
	VI path, walk, d(n), vis(m), ne(n);
	VII st = {{s, -1}};
	d[s]++; // Remove to force cycles
	while (st.size()) {
		auto [x, ex] = st.back();
		int &ei = ne[x], end = adj[x].size(), y, e;
		if (ei == end) { path.push_back(x); walk.push_back(ex); st.pop_back(); continue; }
		tie(y, e) = adj[x][ei++];
		if (!vis[e]) { d[x]--; d[y]++; vis[e] = 1; st.emplace_back(y, e); }
	}
	for (int x: d) if (x < 0 || walk.size() != m) return {{}, {}};
	return {{path.rbegin(), path.rend()}, {walk.rbegin() + 1, walk.rend()}};
}
// end template //
