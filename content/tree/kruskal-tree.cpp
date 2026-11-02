#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numeric>
#include <random>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
using ll = long long;
using ld = long double;
using vi = vector<int>;
#define pb push_back
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)

// begin template //
struct KruskalTree {
	vi p, cnt, roots;
	vector<array<int,2>> ch;
	vector<ll> w;
	KruskalTree(int n, vector<tuple<ll,int,int>> e)
		: p(n,-1), cnt(n,1), ch(n,{-1,-1}), w(n) {
		vi d(n); iota(all(d),0);
		auto f = [&](int u) {
			while (u != d[u]) u = d[u] = d[d[u]];
			return u;
		};
		sort(all(e));
		for (auto [c,u,v] : e) {
			u = f(u); v = f(v); if (u == v) continue;
			int k = sz(p);
			p[u] = p[v] = k; p.pb(-1);
			ch.pb({u,v}); w.pb(c);
			cnt.pb(cnt[u]+cnt[v]);
			d[u] = d[v] = k; d.pb(k);
		}
		rep(i,0,sz(p)) if (p[i] < 0) roots.pb(i);
	}
};
// end template //
