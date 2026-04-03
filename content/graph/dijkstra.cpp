#include <vector>
#include <queue>
#include <random>

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
using pil = pair<int, ll>;
vector<ll> dijkstra(vector<vector<pil>> &g, int s) {
	const ll INF = 1LL << 62;
	vector<ll> dist(sz(g), INF);
	priority_queue<pll> q;
	dist[s] = 0;
	q.push({0, s});
	while (sz(q)) {
		auto [nd, v] = q.top(); q.pop();
		ll d = -nd;
		if (d != dist[v]) continue;
		for (auto [to, w] : g[v]) if (d + w < dist[to]) {
			dist[to] = d + w;
			q.push({-dist[to], to});
		}
	}
	return dist;
}
// end template //
