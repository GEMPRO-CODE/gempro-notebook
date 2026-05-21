#include <iostream>
#include <numeric>
#include <random>
#include <vector>

using namespace std;

using ll = long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;

#define pb push_back
#define eb emplace_back
#define fi first
#define se second
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
#define rep(i, a, b) for (int i = (a); i < (b); i++)

mt19937 rng(random_device{}());

// begin template //
vi matching(vector<vi> &g) {
	int n = sz(g), T = 0;
	vi m(n, -1), p(n), b(n), q(n), u(n), w(n), l(n);
	auto F = [&](int x, int y) {
		for (++T;; x = p[m[x]]) {
			x = b[x];
			l[x] = T;
			if (m[x] < 0) break;
		}
		for (;; y = p[m[y]]) {
			y = b[y];
			if (l[y] == T) return y;
		}
	};
	auto G = [&](int x, int c, int y) {
		for (; b[x] != c; x = p[m[x]])
			w[b[x]] = w[b[m[x]]] = 1, p[x] = y, y = m[x];
	};
	rep(s, 0, n) if (m[s] < 0) {
		fill(all(u), 0), fill(all(p), -1), iota(all(b), 0);
		int a = 0, z = 0, x = -1;
		q[z++] = s, u[s] = 1;
		while (a < z && x < 0) {
			int v = q[a++];
			for (int y : g[v]) {
				if (b[v] == b[y] || m[v] == y)
					continue;
				if (y == s || (~m[y] && ~p[m[y]])) {
					int c = F(v, y);
					fill(all(w), 0);
					G(v, c, y);
					G(y, c, v);
					rep(i, 0, n) if (w[b[i]]) 
						b[i] = c, !u[i] && (u[i] = 1, q[z++] = i);
				} else if (!~p[y]) {
					p[y] = v;
					if (!~m[y]) {
						x = y;
						break;
					}
					y = m[y], u[y] = 1, q[z++] = y;
				}
			}
		}
		while (~x) {
			int y = p[x], v = m[y];
			m[x] = y, m[y] = x, x = v;
		}
	}
	return m;
}
// end template //

int main() { 
	cin.tie(0)->sync_with_stdio(0); 
	int n, m;
	cin >> n >> m;
	vector<vector<int>> g(n);
	for (int i = 0; i < m; i++) {
		int a, b;
		cin >> a >> b;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	auto mat = matching(g);
	vector<pair<int, int>> edges;
	for (int i = 0; i < n; i++) {
		if (mat[i] != -1 && i < mat[i]) edges.emplace_back(i, mat[i]);
	}
	cout << edges.size() << endl;
	for (auto [a, b]: edges) {
		cout << a << ' ' << b << endl;
	}
}
