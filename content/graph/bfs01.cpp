#include <vector>
#include <deque>
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
vi bfs01(vector<vector<pii>> &g, int s) {
	const int INF = 1e9;
	vi dist(sz(g), INF);
	deque<int> q;
	dist[s] = 0;
	q.push_back(s);
	while (sz(q)) {
		int v = q.front(); q.pop_front();
		for (auto [to, w] : g[v]) if (dist[v] + w < dist[to]) {
			dist[to] = dist[v] + w;
			if (w) q.push_back(to);
			else q.push_front(to);
		}
	}
	return dist;
}
// end template //
