#include <vector>
#include <random>
#include <iostream>
#include <algorithm>

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
#define rep(i,a,b) for (int i = (a); i < (b); ++i)

mt19937 rng(random_device{}());

// begin template //
vi treeMatching(vector<vi> &g, int root = 0) {
	int n = sz(g);
	vi par(n, -1), ord{root}, mat(n, -1);
	rep(i,0,sz(ord)) {
		int v = ord[i];
		for (int u : g[v]) if (u != par[v]) {
			par[u] = v;
			ord.pb(u);
		}
	}
	for (int i = n - 1; i > 0; --i) {
		int v = ord[i], p = par[v];
		if (mat[v] == -1 && mat[p] == -1)
			mat[v] = p, mat[p] = v;
	}
	return mat;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n;
	cin >> n;
	vector<vi> g(n);
	for (int i = 1; i < n; i++) {
		int a, b;
		cin >> a >> b;
		a--,b--;
		g[a].pb(b);
		g[b].pb(a);
	}
	auto mat = treeMatching(g);
	cout << (n - count(all(mat), -1)) / 2 << endl;
}
