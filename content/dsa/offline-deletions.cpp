#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <cassert>

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

// begin template //
template<class DS>
struct OfflineDeletion {
	using V = typename DS::Value;
	int t = 0, s = 1;
	map<V, int> on;
	vector<tuple<int, int, V>> in;
	vector<vector<V>> st;
	void insert(V x) { 
		if (!on.count(x)) on[x] = t; 
		t++;
	}
	void remove(V x) {
		auto it = on.find(x);
		if (it != on.end()) 
			in.eb(it->se, t, x), on.erase(it);
		t++;
	}
	void add(int l, int r, V x) {
		for (l += s, r += s; l < r; l >>= 1, r >>= 1) {
			if (l & 1) st[l++].pb(x);
			if (r & 1) st[--r].pb(x);
		}
	}
	void dfs(DS &ds, int i = 1, int l = 0, int r = -1) {
		if (r < 0) r = s;
		int c = sz(st[i]);
		for (V x: st[i]) ds.insert(x);
		if (r - l == 1) {
			if (l < t) ds.handle(r);
		} else {
			int m = (l + r) / 2;
			dfs(ds, i << 1, l, m);
			dfs(ds, i << 1 | 1, m, r);
		}
		while (c--) ds.rollback();
	}
	void run(DS &ds) {
		for (auto [x, l]: on) in.eb(l, t, x);
		on.clear();
		while (s < t) s <<= 1;
		st.assign(2 * s, {});
		for (auto [l, r, x]: in) if (l < r) add(l, r, x);
		dfs(ds);
	}
};
// end template //

struct DSU {
	vi par, r;
	struct Change {
		int x, y, rx;
	};
	vector<Change> changes;
	DSU(int n) : par(n), r(n) { iota(all(par), 0); }
	int root(int x) {
		return x == par[x] ? x : root(par[x]);
	}
	bool merge(int x, int y) {
		x = root(x), y = root(y);
		if (x == y) {
			changes.eb(-1, -1, -1);
			return 0;
		}
		if (r[x] < r[y]) swap(x, y);
		changes.eb(x, y, r[x]);
		par[y] = x;
		if (r[x] == r[y]) r[x]++;
		return 1;
	}
	bool rollback() {
		auto [x, y, rx] = changes.back();
		changes.pop_back();
		if (x == -1) return 0;
		r[x] = rx;
		par[y] = y;
		return 1;
	}
};

template<class F>
struct Connectivity {
	DSU dsu;
	F callback;
	int comps;
	using Value = pair<int, int>;
	Connectivity(int n, F callback) : dsu(n), callback(callback), comps(n) {}
	void insert(Value e) {
		comps -= dsu.merge(e.fi, e.se);
	}
	void rollback() {
		comps += dsu.rollback();
	}
	void handle(int r) {
		callback(r, comps);
	}
};

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n, m, k;
	cin >> n >> m >> k;
	Connectivity con(n, [&](int r, int comps) {
		if (r >= m) cout << comps << ' ';
	});
	OfflineDeletion<decltype(con)> dynCon;
	for (int i = 0; i < m; i++) {
		int a, b;
		cin >> a >> b;
		a--, b--;
		if (a > b) swap(a, b);
		dynCon.insert({a, b});
	}
	for (int i = 0; i < k; i++) {
		int t, a, b;
		cin >> t >> a >> b;
		a--, b--;
		if (a > b) swap(a, b);
		if (t == 1) {
			dynCon.insert({a, b});
		} else if (t == 2) {
			dynCon.remove({a, b});
		} else assert(false); 
	}
	dynCon.run(con);
	cout << endl;
}

