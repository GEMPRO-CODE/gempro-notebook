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
vector<pii> havelHakimi(vi deg) {
	int n = sz(deg), mx = n;
	vector<pii> ed;
	ed.reserve(accumulate(all(deg), 0LL) / 2);
	vi head(n + 1, -1), nxt(n, -1), prv(n, -1), take;
	auto rem = [&](int v) {
		int d = deg[v], p = prv[v], q = nxt[v];
		if (p == -1) head[d] = q;
		else nxt[p] = q;
		if (q != -1) prv[q] = p;
		prv[v] = nxt[v] = -1;
	};
	auto ins = [&](int v) {
		int d = deg[v];
		prv[v] = -1, nxt[v] = head[d];
		if (head[d] != -1) prv[head[d]] = v;
		head[d] = v;
	};
	rep(i,0,n) if (deg[i]) ins(i);
	while (mx && head[mx] == -1) --mx;
	while (mx) {
		int v = head[mx];
		rem(v);
		int d = deg[v];
		take.clear(), take.reserve(d);
		int cur = mx;
		rep(_,0,d) {
			while (cur && head[cur] == -1) --cur;
			int u = head[cur], rem(u);
			take.pb(u);
		}
		for (int u : take) ed.eb(v, u), --deg[u];
		deg[v] = 0;
		for (int u : take) if (deg[u]) ins(u);
		while (mx && head[mx] == -1) --mx;
	}
	return ed;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}

