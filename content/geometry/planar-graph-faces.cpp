#include <iostream>
#include <vector>
#include <random>
#include <complex>
#include <map>
#include <algorithm>

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

using Pt = complex<ll>;
#define xx real()
#define yy imag()

ll cross(Pt a, Pt b) { return (conj(a) * b).yy; }

// begin template //
ll half(Pt p) {
	return p.yy < 0 || (p.yy == 0 && p.xx < 0);
}

ll norm2(Pt a) {
	return a.xx * a.xx + a.yy * a.yy;
}

vector<vi> faces(vector<Pt> &p, vector<vi> g) {
	int n = sz(g);
	vector<map<int, int>> id(n);
	rep(i,0,n) {
		sort(all(g[i]), [&](int a, int b) {
			Pt A = p[a] - p[i], B = p[b] - p[i];
			if (half(A) != half(B)) return half(A) < half(B);
			auto c = cross(A, B);
			if (c) return c > 0;
			return norm2(A) < norm2(B); // only tie safety
		});
		rep(j,0,sz(g[i])) id[i][g[i][j]] = j;
	}
	vector<vector<char>> vis(n);
	rep(i,0,n) vis[i].assign(sz(g[i]), 0);
	vector<vi> fs;
	rep(s,0,n) rep(e,0,sz(g[s])) if (!vis[s][e]) {
		vi f;
		int u = s, i = e;
		while (!vis[u][i]) {
			vis[u][i] = 1;
			int v = g[u][i];
			f.pb(u);
			int j = id[v][u];
			u = v;
			i = (j - 1 + sz(g[u])) % sz(g[u]);
		}
		fs.pb(f);
	}
	return fs;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
