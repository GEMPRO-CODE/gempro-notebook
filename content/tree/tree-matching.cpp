#include <vector>
#include <functional>

using namespace std;

using VI = vector<int>;
using VVI = vector<VI>;

// begin template //
// Maximum matching on tree (0-indexed adjacency list)
// Returns matching size; match[v] = matched partner or -1
int treeMatch(VVI &adj, VI &match, int root = 0) {
	int n = adj.size(), ans = 0;
	match.assign(n, -1);
	function<bool(int, int)> dfs = [&](int v, int p) -> bool {
		for (int u: adj[v]) if (u != p && dfs(u, v) && match[v] == -1) {
			match[v] = u; match[u] = v; ans++;
		}
		return match[v] == -1;
	};
	dfs(root, -1);
	return ans;
}
// end template //

// Test at https://cses.fi/problemset/task/1130/

#include <iostream>
int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n; cin >> n;
	VVI adj(n);
	for (int i = 0; i < n - 1; i++) {
		int a, b; cin >> a >> b; a--; b--;
		adj[a].push_back(b); adj[b].push_back(a);
	}
	VI match;
	cout << treeMatch(adj, match) << "\n";
}
