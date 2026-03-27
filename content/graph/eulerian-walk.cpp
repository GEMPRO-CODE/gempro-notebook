#include <vector>
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
#define sz(x) ssize(x)
#define rep(i,a,b) for (int i = (a); i < (b); ++i)

// begin template //
pair<vi, vi> eulerWalk(vector<vector<pii>>& g, int m, int s = 0) {
    int n = sz(g);
    vi path, walk, d(n), vis(m), it(n);
    vector<pii> st = {{s, -1}};
    d[s]++; // remove to force cycles
    while (sz(st)) {
        auto [v, e] = st.back();
        if (it[v] == sz(g[v])) {
            path.pb(v), walk.pb(e);
            st.pop_back();
        } else {
            auto [u, id] = g[v][it[v]++];
            if (!vis[id]) {
                d[v]--, d[u]++;
                vis[id] = 1;
                st.pb({u, id});
            }
        }
    }
    for (int x : d) if (x < 0) return {{}, {}};
    if (sz(walk) != m + 1) return {{}, {}};
    return { vi(path.rbegin(), path.rend()), vi(walk.rbegin() + 1, walk.rend()) };
}
// end template //

void directed() {
	int t;
	cin >> t;
	auto solve = [&]() {
		int n, m;
		cin >> n >> m;
		vector<vector<pii>> g(n);
		vi in(n), out(n);
		rep(i, 0, m) {
			int a, b;
			cin >> a >> b;
			g[a].eb(b, i);
			out[a]++, in[b]++;
		}
		if (m == 0) {
			cout << "Yes" << endl << 0 << endl << endl;
			return;
		}
		int s = -1;
		rep(i, 0, n) {
			if (out[i] - in[i] == 1) {
				s = i;
			}
		}
		if (s == -1) {
			rep(i, 0, n) {
				if (out[i] > 0) {
					s = i;
				}
			}
		}
		auto [v, e] = eulerWalk(g, m, s);
		if (sz(e) == m) {
			cout << "Yes" << endl;
			for (int i: v) cout << i << ' ';
			cout << endl;
			for (int i: e) cout << i << ' ';
			cout << endl;
		} else cout << "No" << endl;
	};
	while (t--) solve();
}

void undirected() {
	int t;
	cin >> t;

	auto solve = [&]() {
		int n, m;
		cin >> n >> m;
		vector<vector<pii>> g(n);
		if (m == 0) {
			cout << "Yes" << endl << 0 << endl << endl;
			return;
		}
		rep(i, 0, m) {
			int a, b;
			cin >> a >> b;
			g[a].eb(b, i);
			g[b].eb(a, i);
		}
		int s = -1;
		rep(i, 0, n) {
			if (sz(g[i]) & 1) {
				s = i;
			}
		}
		if (s == -1) {
			rep(i, 0, n) if (sz(g[i])) s = i;
		}
		auto [v, e] = eulerWalk(g, m, s);
		if (e.size() == m) {
			cout << "Yes" << endl;
			for (int i: v) cout << i << ' ';
			cout << endl;
			for (int i: e) cout << i << ' ';
			cout << endl;
		} else cout << "No" << endl;
	};
	while (t--) solve();
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	// directed();
	undirected();
}
