#include <vector>
#include <random>
#include <algorithm>
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
vi bridges(vector<vi> &g) {
	int n = sz(g), ti = 0, cc = 0;
	vi tin(n), low(n), st, comp;
	comp.assign(n, -1);
	auto make = [&](int v) {
		while (1) {
			int x = st.back(); st.pop_back();
			comp[x] = cc;
			if (x == v) break;
		}
		cc++;
	};
	auto dfs = [&](auto self, int v, int p) -> void {
		tin[v] = low[v] = ++ti;
		st.pb(v);
		int usedPar = 0;
		for (int to : g[v]) {
			if (to == p && !usedPar) { usedPar = 1; continue; }
			if (!tin[to]) {
				self(self, to, v);
				low[v] = min(low[v], low[to]);
				if (low[to] > tin[v]) make(to);
			} else low[v] = min(low[v], tin[to]);
		}
	};
	rep(i,0,n) if (!tin[i]) {
		dfs(dfs, i, -1);
		make(i);
	}
	return comp;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
