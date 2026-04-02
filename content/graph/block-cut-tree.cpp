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
vector<vi> bct(vector<vi> &g) {
	int n = sz(g), ti = 0;
	vector<vi> t(n);
	vi num(n), low(n), seen(n);
	vector<pii> st;
	auto add = [&](int a, int b) {
		int id = sz(t), it = id + 1;
		t.pb({});
		while (1) {
			auto [x, y] = st.back(); st.pop_back();
			if (seen[x] != it) seen[x] = it, t[x].pb(id), t[id].pb(x);
			if (seen[y] != it) seen[y] = it, t[y].pb(id), t[id].pb(y);
			if (x == a && y == b) break;
		}
	};
	auto dfs = [&](auto self, int v, int p) -> void {
		num[v] = low[v] = ++ti;
		int ch = 0;
		for (int to : g[v]) if (to != p) {
			if (!num[to]) {
				st.eb(v, to);
				ch++;
				self(self, to, v);
				low[v] = min(low[v], low[to]);
				if (low[to] >= num[v]) add(v, to);
			} else if (num[to] < num[v]) {
				st.eb(v, to);
				low[v] = min(low[v], num[to]);
			}
		}
		if (p == -1 && !ch) t.pb({v}), t[v].pb(sz(t) - 1);
	};
	rep(i, 0, n) if (!num[i]) dfs(dfs, i, -1);
	return t;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
