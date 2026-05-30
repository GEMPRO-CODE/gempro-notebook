#include <vector>
#include <random>
#include <iostream>
#include <list>
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
#define rep(i,a,b) for (int i = (a); i < (b); i++)

mt19937 rng(random_device{}());

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

// begin template //
vi stNumber(vector<vi> &g, int s, int t) {
	int n = sz(g), ti = 0;
	vi in(n, -1), low(n), p(n, -1), by(n), sig(n, -1), num(n);
	auto dfs = [&](auto &&self, int v) -> void {
		in[v] = low[v] = ti, by[ti++] = v;
		auto go = [&](int u) {
			if (u == p[v]) return;
			if (in[u] < 0) p[u] = v, self(self, u), low[v] = min(low[v], low[u]);
			else low[v] = min(low[v], in[u]);
		};
		if (v == s && in[t] < 0) p[t] = s, self(self, t); // virtual edge
		for (int u : g[v]) if (v != s || u != t) go(u);
	};
	dfs(dfs, s);
	list<int> l = {s, t};
	vector<list<int>::iterator> it(n);
	it[s] = l.begin(), it[t] = next(l.begin());
	rep(i,2,n) {
		int v = by[i], q = p[v], x = by[low[v]];
		if (sig[x] < 0) it[v] = l.insert(it[q], v), sig[q] = 1;
		else it[v] = l.insert(next(it[q]), v), sig[q] = -1;
	}
	int i = 0;
	for (int v : l) num[v] = i++;
	return num;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int t;
	cin >> t;
	while (t--) {
		int n, m, s, t;
		cin >> n >> m >> s >> t;
		vector<vector<int>> g(n);
		if (n == 2 && m == 0) {
			cout << "No" << endl;
			continue;
		}
		for (int i = 0; i < m; i++) {
			int a, b;
			cin >> a >> b;
			g[a].push_back(b);
			g[b].push_back(a);
		}
		bool ok = true;
		{ // check existence
			if (find(begin(g[s]), end(g[s]), t) != end(g[s])) {
				auto tr = bct(g);
				if (sz(tr) > sz(g) + 1) ok = false;
			} else {
				g[s].push_back(t);
				g[t].push_back(s);
				auto tr = bct(g);
				if (sz(tr) > sz(g) + 1) ok = false;
				g[s].pop_back();
				g[t].pop_back();
			}
		}
		if (!ok) {
			cout << "No" << endl;
			continue;
		}
		auto st = stNumber(g, s, t);
		cout << "Yes" << endl;
		for (int i = 0; i < n; i++) {
			cout << st[i] << ' ';
		}
		cout << endl;
	}
}
