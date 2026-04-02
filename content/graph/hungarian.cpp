#include <vector>
#include <random>
#include <algorithm>
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
pair<ll, vi> hungarian(vector<vector<ll>> a) {
	int n = sz(a), m = sz(a[0]);
	const ll INF = 4e18;
	vector<ll> u(n + 1), v(m + 1), minv(m + 1);
	vi p(m + 1), way(m + 1), ans(n);
	rep(i, 1, n + 1) {
		p[0] = i;
		int j0 = 0;
		fill(all(minv), INF);
		vector<char> used(m + 1);
		do {
			used[j0] = 1;
			int i0 = p[j0], j1 = 0;
			ll delta = INF;
			rep(j, 1, m + 1) if (!used[j]) {
				ll cur = a[i0 - 1][j - 1] - u[i0] - v[j];
				if (cur < minv[j]) minv[j] = cur, way[j] = j0;
				if (minv[j] < delta) delta = minv[j], j1 = j;
			}
			rep(j, 0, m + 1)
				if (used[j]) u[p[j]] += delta, v[j] -= delta;
				else minv[j] -= delta;
			j0 = j1;
		} while (p[j0]);
		do {
			int j1 = way[j0];
			p[j0] = p[j1];
			j0 = j1;
		} while (j0);
	}
	rep(j, 1, m + 1) if (p[j]) ans[p[j] - 1] = j - 1;
	return {-v[0], ans};
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n;
	cin >> n;
	vector<vector<ll>> a(n, vector<ll>(n));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> a[i][j];
		}
	}
	auto [cost, mat] = hungarian(a);
	cout << cost << endl;
	for (int i = 0; i < n; i++) {
		cout << mat[i] << ' ';
	}
	cout << endl;
}
