#include <vector>
#include <random>
#include <iostream>
#include <climits>

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
struct E { int a, b; ll w; };
struct H { int a; ll w, d = 0; H *l = 0, *r = 0; };
void P(H *x) {
	if (!x || !x->d) return;
	x->w += x->d;
	if (x->l) x->l->d += x->d;
	if (x->r) x->r->d += x->d;
	x->d = 0;
}
H* M(H *a, H *b) {
	if (!a || !b) return a ? a : b;
	P(a), P(b);
	if (a->w > b->w) swap(a, b);
	a->r = M(a->r, b);
	swap(a->l, a->r);
	return a;
}
H* pop(H *x) {
	P(x);
	return M(x->l, x->r);
}

ll dmst(int n, int r, vector<E> es) {
	vector<H*> h(n);
	for (auto e : es) h[e.b] = M(h[e.b], new H{e.a, e.w});
	vi uf(n), se(n, -1);
	iota(all(uf), 0);
	auto f = [&](auto &&f, int x) -> int {
		return uf[x] == x ? x : uf[x] = f(f, uf[x]);
	};
	se[r] = r;
	vector<pair<int, ll>> st;
	ll ans = 0;
	rep(s,0,n) {
		int u = f(f, s);
		if (se[u] >= 0) continue;
		st.clear();
		while (se[u] < 0) {
			se[u] = s;
			while (h[u] && f(f, h[u]->a) == u) h[u] = pop(h[u]);
			if (!h[u]) return LLONG_MAX;
			P(h[u]);
			ll c = h[u]->w;
			ans += c;
			st.eb(u, c);
			u = f(f, h[u]->a);
			if (se[u] == s) {
				H *x = 0;
				int v = u;
				for (;;) {
					auto [w, c] = st.back();
					st.pop_back();
					h[w]->d -= c;
					x = M(x, h[w]);
					uf[w] = v;
					if (w == v) break;
				}
				u = v;
				h[u] = x;
				se[u] = -1;
			}
		}
	}
	return ans;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
