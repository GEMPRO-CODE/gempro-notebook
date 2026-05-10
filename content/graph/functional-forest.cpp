#include <vector>
#include <queue>
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
vi funcForest(vi &f) {
	vi deg(sz(f)), root(sz(f), -1), ord;
	for (int v : f) deg[v]++;
	queue<int> q;
	rep (i, 0, sz(f)) if (!deg[i]) q.push(i);
	while (sz(q)) {
		int v = q.front(); q.pop();
		ord.pb(v);
		if (!--deg[f[v]]) q.push(f[v]);
	}
	rep (i, 0, sz(f)) if (deg[i]) {
		root[i] = i;
		for (int v = f[i]; v != i; v = f[v]) root[v] = i, deg[v] = 0;
		deg[i] = 0;
	}
	for (int i = sz(ord) - 1; i >= 0; --i) root[ord[i]] = root[f[ord[i]]];
	return root;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
