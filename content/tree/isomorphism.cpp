#include <vector>
#include <map>
#include <algorithm>
#include <utility>
using namespace std;
using vi = vector<int>;

// begin template //
struct TreeIso {
	map<vi,int> ids;
	vi classify(vector<vi> &g, int r = 0) {
		int n = g.size();
		vi p(n, -1), ord{r}, ans(n);
		for (int i = 0; i < n; ++i)
			for (int v : g[ord[i]]) if (v != p[ord[i]])
				p[v] = ord[i], ord.push_back(v);
		for (int i = n-1; i >= 0; --i) {
			int u = ord[i];
			vi a;
			for (int v : g[u]) if (v != p[u]) a.push_back(ans[v]);
			sort(a.begin(),a.end());
			ans[u] = ids.emplace(move(a),ids.size()).first->second;
		}
		return ans;
	}
};
// end template //
