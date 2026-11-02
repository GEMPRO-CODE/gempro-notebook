#include <vector>
#include <algorithm>
using namespace std;
using vi = vector<int>;

// begin template //
template<class T>
vector<T> reroot(vector<vi> &g, T e, auto merge, auto vertex, auto edge) {
	int n = g.size();
	vi p(n, -1), ord{0};
	vector<T> down(n, e), up(n, e), ans(n);
	for (int i = 0; i < n; ++i)
		for (int v : g[ord[i]]) if (v != p[ord[i]])
			p[v] = ord[i], ord.push_back(v);
	for (int i = n-1; i >= 0; --i) {
		int u = ord[i];
		T x = e;
		for (int v : g[u]) if (v != p[u])
			x = merge(x, edge(down[v], v, u));
		down[u] = vertex(x, u);
	}
	for (int u : ord) {
		int m = g[u].size();
		vector<T> a(m), suf(m+1, e);
		for (int i = 0; i < m; ++i) {
			int v = g[u][i];
			a[i] = v == p[u] ? up[u] : edge(down[v], v, u);
		}
		for (int i = m-1; i >= 0; --i) suf[i] = merge(a[i], suf[i+1]);
		ans[u] = vertex(suf[0], u);
		T pre = e;
		for (int i = 0; i < m; ++i) {
			int v = g[u][i];
			if (v != p[u]) up[v] = edge(vertex(merge(pre, suf[i+1]), u), u, v);
			pre = merge(pre, a[i]);
		}
	}
	return ans;
}
// end template //
