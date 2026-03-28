#include <climits>
#include <iostream>
#include <queue>
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
#define rep(i,a,b) for (int i = (a); i < (b); ++i)

mt19937 rng(random_device{}());

// begin template //
struct MCMF {
	struct E { int to, rev; ll cap, cost; };
	int n;
	vector<vector<E>> g;
	MCMF(int n) : n(n), g(n) {}
	void addEdge(int u, int v, ll cap, ll cost) {
		g[u].eb(v, sz(g[v]), cap, cost);
		g[v].eb(u, sz(g[u]) - 1, 0, -cost);
	}
	pll flow(int s, int t, ll k = 4e18) {
		const ll INF = 4e18;
		ll fl = 0, cost = 0;
		vector<ll> p(n, INF), d(n), f(n);
		vi pv(n), pe(n), inq(n);
		queue<int> q;
		p[s] = 0, q.push(s), inq[s] = 1;
		while (sz(q)) {
			int u = q.front(); q.pop();
			inq[u] = 0;
			rep(i,0,sz(g[u])) {
				auto &e = g[u][i];
				if (e.cap && p[e.to] > p[u] + e.cost) {
					p[e.to] = p[u] + e.cost;
					if (!inq[e.to]) q.push(e.to), inq[e.to] = 1;
				}
			}
		}
		rep(i,0,n) if (p[i] == INF) p[i] = 0;
		while (fl < k) {
			fill(all(d), INF);
			priority_queue<pll, vector<pll>, greater<pll>> pq;
			d[s] = 0, f[s] = k - fl, pq.emplace(0, s);
			while (sz(pq)) {
				auto [du, u] = pq.top(); pq.pop();
				if (du != d[u]) continue;
				rep(i,0,sz(g[u])) {
					auto &e = g[u][i];
					if (!e.cap) continue;
					ll nd = du + e.cost + p[u] - p[e.to];
					if (d[e.to] > nd) {
						d[e.to] = nd, pv[e.to] = u, pe[e.to] = i;
						f[e.to] = min(f[u], e.cap);
						pq.emplace(nd, e.to);
					}
				}
			}
			if (d[t] == INF) break;
			rep(i,0,n) if (d[i] < INF) p[i] += d[i];
			ll x = f[t];
			fl += x, cost += x * p[t];
			for (int v = t; v != s; v = pv[v]) {
				auto &e = g[pv[v]][pe[v]];
				e.cap -= x, g[v][e.rev].cap += x;
			}
		}
		return {fl, cost};
	}
};
// end template //

// Test at https://cses.fi/problemset/task/2129/

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n; cin >> n;
	MCMF g(2 * n + 2);
	int s = 2 * n, t = 2 * n + 1;
	for (int i = 0; i < n; i++) g.addEdge(s, i, 1, 0);
	for (int i = 0; i < n; i++) g.addEdge(n + i, t, 1, 0);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			int c; cin >> c;
			g.addEdge(i, n + j, 1, c);
		}
	auto [f, c] = g.flow(s, t);
	cout << c << "\n";
}
