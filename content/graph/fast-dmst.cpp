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
struct DE { int a, b; ll w; };
struct DN {
	DE e; ll d = 0;
	DN *l = 0, *r = 0;
	DN(DE e) : e(e) {}
};
void prop(DN *x) {
	if (!x || !x->d) return;
	x->e.w += x->d;
	if (x->l) x->l->d += x->d;
	if (x->r) x->r->d += x->d;
	x->d = 0;
}
DN* meld(DN *a, DN *b) {
	if (!a || !b) return a ? a : b;
	prop(a), prop(b);
	if (a->e.w > b->e.w) swap(a, b);
	a->r = meld(a->r, b);
	swap(a->l, a->r);
	return a;
}
DN* pop(DN *a) { prop(a); return meld(a->l, a->r); }

ll dmst(int n, int r, vector<DE> es) {
	vector<DN*> h(n);
	for (auto e : es) if (e.a != e.b) h[e.b] = meld(h[e.b], new DN(e));
	vi uf(n), seen(n, -1), path;
	vector<ll> in(n);
	iota(all(uf), 0);
	auto find = [&](auto &&self, int x) -> int {
		return uf[x] == x ? x : uf[x] = self(self, uf[x]);
	};
	auto join = [&](int a, int b) {
		a = find(find, a), b = find(find, b);
		if (a == b) return 0;
		uf[b] = a;
		return 1;
	};
	seen[r] = r;
	ll ans = 0;
	rep (s, 0, n) {
		int u = find(find, s);
		if (u == r || seen[u] >= 0) continue;
		path.clear();
		while (seen[u] < 0) {
			path.pb(u);
			seen[u] = s;
			while (h[u] && find(find, h[u]->e.a) == find(find, h[u]->e.b))
				h[u] = pop(h[u]);
			if (!h[u]) return -1;
			prop(h[u]);
			ans += in[u] = h[u]->e.w;
			u = find(find, h[u]->e.a);
			if (seen[u] == s) {
				int v = u;
				DN *x = 0;
				while (1) {
					int w = path.back();
					path.pop_back();
					h[w]->d -= in[w];
					x = meld(x, h[w]);
					join(v, w);
					if (w == v) break;
				}
				u = find(find, v);
				h[u] = x;
				seen[u] = -1;
			}
		}
	}
	return ans;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
