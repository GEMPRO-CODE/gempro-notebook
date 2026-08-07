#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <random>
#include <vector>
#include <utility>

using namespace std;

using ll = long long;
using ld = double;
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

namespace fft {
using cd = complex<ld>;
const ld PI = acosl(-1.0L);

void fft(vector<cd>& a) {
	int n = sz(a);
	static vector<cd> rt(2, 1);
	for (static int k = 2; k < n; k <<= 1) {
		rt.resize(n);
		cd x = polar((ld)1, PI / k);
		rep(i, k, 2 * k) rt[i] = i & 1 ? rt[i / 2] * x : rt[i / 2];
	}
	for (int i = 1, j = 0; i < n; i++) {
		int bit = n >> 1;
		for (; j & bit; bit >>= 1) j ^= bit;
		j ^= bit;
		if (i < j) swap(a[i], a[j]);
	}
	for (int k = 1; k < n; k <<= 1)
		for (int i = 0; i < n; i += k << 1)
			rep(j, 0, k) {
				cd z = rt[j + k] * a[i + j + k];
				a[i + j + k] = a[i + j] - z;
				a[i + j] += z;
			}
}
}

vector<ll> conv(vector<ll> a, vector<ll> b) {
	if (a.empty() || b.empty()) return {};
	if (ssize(a) > ssize(b)) swap(a, b);
	if (ssize(a) < 16) {
		int n = ssize(a), m = ssize(b);
		vector<ll> ans(n + m - 1);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				ans[i + j] += a[i] * b[j];
			}
		}
		return ans;
	}
	int s = sz(a) + sz(b) - 1, n = 1;
	while (n < s) n <<= 1;
	vector<fft::cd> in(n), out(n);
	rep(i, 0, sz(a)) in[i].real(a[i]);
	rep(i, 0, sz(b)) in[i].imag(b[i]);
	fft::fft(in);
	for (fft::cd& x : in) x *= x;
	rep(i, 0, n) out[i] = in[-i & (n - 1)] - conj(in[i]);
	fft::fft(out);
	vector<ll> res(s);
	rep(i, 0, s) res[i] = llround(imag(out[i]) / (4 * n));
	return res;
}

// begin template //
vector<ll> treeDistHist(vector<vi>& g) {
	int n = sz(g);
	vi sub(n), dead(n);
	vector<ll> ans(n);
	auto solve = [&](auto&& self, int s) -> void {
		auto dfs = [&](auto&& dfs, int u, int p) -> int {
			sub[u] = 1;
			for (int v : g[u]) if (v != p && !dead[v])
				sub[u] += dfs(dfs, v, u);
			return sub[u];
		};
		int N = dfs(dfs, s, -1), c = s, p = -1;
		while (1) {
			int v = -1;
			for (int x : g[c]) if (x != p && !dead[x] && sub[x] > N / 2)
				v = x;
			if (v == -1) break;
			p = c, c = v;
		}
		vector<ll> a(1, 1);
		for (int v : g[c]) if (!dead[v]) {
			vector<ll> b(1);
			auto dep = [&](auto&& dep, int u, int p, int d) -> void {
				if (sz(b) <= d) b.resize(d + 1);
				b[d]++;
				for (int v : g[u]) if (v != p && !dead[v]) dep(dep, v, u, d + 1);
			};
			dep(dep, v, c, 1);
			if (sz(a) < sz(b)) a.resize(sz(b));
			rep(i, 1, sz(b)) a[i] += b[i];
			auto q = conv(b, b);
			rep(i, 1, min(n, sz(q))) ans[i] -= q[i];
		}
		auto q = conv(a, a);
		rep(i, 1, min(n, sz(q))) ans[i] += q[i];
		dead[c] = 1;
		for (int v : g[c]) if (!dead[v]) self(self, v);
	};
	solve(solve, 0);
	rep(i, 1, n) ans[i] /= 2;
	return ans;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n;
	cin >> n;
	vector<vector<int>> g(n);
	for (int i = 1; i < n; i++) {
		int u, v;
		cin >> u >> v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	auto hist = treeDistHist(g);
	for (int i = 1; i < n; i++) cout << hist[i] << ' ';
	cout << endl;
}
