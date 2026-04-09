#include <vector>
#include <random>
#include <iostream>

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
vi bfs(vector<vi> &g, vi &p, int s) {
	int n = sz(g);
	vi ord = {s};
	p[s] = s;
	rep(i, 0, sz(ord)) {
		int t = ord[i];
		for (int j: g[t]) if (p[j] == -1) p[j] = t, ord.pb(j);
	}
	return ord;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
