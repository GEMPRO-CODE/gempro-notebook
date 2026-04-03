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
struct E {
	int a, b;
	ll w;
};

ll dmst(int n, int r, vector<E> e) {
	const ll INF = 1LL << 62;
	ll ans = 0;
	while (1) {
		vector<ll> in(n, INF);
		vi pre(n, -1), id(n, -1), vis(n, -1);
		for (auto [a, b, w] : e) if (a != b && w < in[b]) in[b] = w, pre[b] = a;
		in[r] = 0;
		rep (i, 0, n) if (in[i] == INF) return -1;
		int cnt = 0;
		rep (i, 0, n) {
			ans += in[i];
			int v = i;
			while (vis[v] != i && id[v] == -1 && v != r) vis[v] = i, v = pre[v];
			if (v != r && id[v] == -1) {
				id[v] = cnt;
				for (int u = pre[v]; u != v; u = pre[u]) id[u] = cnt;
				cnt++;
			}
		}
		if (!cnt) break;
		rep (i, 0, n) if (id[i] == -1) id[i] = cnt++;
		vector<E> ne;
		for (auto [a, b, w] : e) {
			int x = id[a], y = id[b];
			if (x != y) ne.pb({x, y, w - in[b]});
		}
		r = id[r], n = cnt;
		e.swap(ne);
	}
	return ans;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
