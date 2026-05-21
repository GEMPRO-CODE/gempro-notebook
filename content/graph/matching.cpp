#include <iostream>
#include <vector>
#include <random>
#include <numeric>

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
#define rep(i,a,b) for (int i = (a); i < (b); i++)

mt19937 rng(random_device{}());

// begin template //
vi blossom(vector<vi> &g) {
	int n = sz(g), t = 0;
	vi mt(n, -1), p(n), base(n), q(n), inq(n), inb(n), inp(n);

	auto lca = [&](int a, int b) {
		++t;
		while (1) {
			a = base[a];
			inp[a] = t;
			if (mt[a] == -1) break;
			a = p[mt[a]];
		}
		while (1) {
			b = base[b];
			if (inp[b] == t) return b;
			b = p[mt[b]];
		}
	};

	auto mark = [&](int v, int b, int x) {
		while (base[v] != b) {
			inb[base[v]] = inb[base[mt[v]]] = 1;
			p[v] = x;
			x = mt[v];
			v = p[mt[v]];
		}
	};

	auto aug = [&](int s) {
		fill(all(inq), 0);
		fill(all(p), -1);
		iota(all(base), 0);
		int l = 0, r = 0;
		q[r++] = s;
		inq[s] = 1;
		while (l < r) {
			int v = q[l++];
			for (int u : g[v]) {
				if (base[v] == base[u] || mt[v] == u) continue;
				if (u == s || (mt[u] != -1 && p[mt[u]] != -1)) {
					int b = lca(v, u);
					fill(all(inb), 0);
					mark(v, b, u);
					mark(u, b, v);
					rep(i,0,n) if (inb[base[i]]) {
						base[i] = b;
						if (!inq[i]) inq[i] = 1, q[r++] = i;
					}
				} else if (p[u] == -1) {
					p[u] = v;
					if (mt[u] == -1) {
						while (u != -1) {
							v = p[u];
							int w = v == -1 ? -1 : mt[v];
							mt[u] = v;
							if (v != -1) mt[v] = u;
							u = w;
						}
						return 1;
					}
					u = mt[u];
					inq[u] = 1;
					q[r++] = u;
				}
			}
		}
		return 0;
	};

	int greedy = 1;
	while (greedy) {
		greedy = 0;
		rep(i,0,n) if (mt[i] == -1)
			for (int j : g[i]) if (mt[j] == -1) {
				mt[i] = j, mt[j] = i;
				greedy = 1;
				break;
			}
	}
	rep(i,0,n) if (mt[i] == -1) aug(i);
	return mt;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
