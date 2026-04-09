#include <vector>
#include <iostream>
#include <random>

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
struct Simplex {
	static constexpr ld EPS = 1e-10;
	static constexpr ld INF = 1e100;
	int m, n;
	vi B, N;
	vector<vector<ld>> D;
	Simplex(vector<vector<ld>> A, vector<ld> b, vector<ld> c)
		: m(sz(b)), n(sz(c)), B(m), N(n + 1), D(m + 2, vector<ld>(n + 2)) {
		rep(i,0,m) rep(j,0,n) D[i][j] = A[i][j];
		rep(i,0,m) B[i] = n + i, D[i][n] = -1, D[i][n + 1] = b[i];
		rep(j,0,n) N[j] = j, D[m][j] = -c[j];
		N[n] = -1, D[m + 1][n] = 1;
	}
	void pivot(int r, int s) {
		ld inv = 1 / D[r][s];
		rep(i,0,m+2) if (i != r)
			rep(j,0,n+2) if (j != s) D[i][j] -= D[r][j] * D[i][s] * inv;
		rep(j,0,n+2) if (j != s) D[r][j] *= inv;
		rep(i,0,m+2) if (i != r) D[i][s] *= -inv;
		D[r][s] = inv;
		swap(B[r], N[s]);
	}
	bool simplex(int ph) {
		int x = ph == 1 ? m + 1 : m;
		while (1) {
			int s = -1;
			rep(j,0,n+1) if (!(ph == 2 && N[j] == -1) &&
				(s == -1 || D[x][j] < D[x][s] - EPS ||
				(abs(D[x][j] - D[x][s]) <= EPS && N[j] < N[s]))) s = j;
			if (D[x][s] >= -EPS) return 1;
			int r = -1;
			rep(i,0,m) if (D[i][s] > EPS &&
				(r == -1 || D[i][n + 1] / D[i][s] < D[r][n + 1] / D[r][s] - EPS ||
				(abs(D[i][n + 1] / D[i][s] - D[r][n + 1] / D[r][s]) <= EPS && B[i] < B[r]))) r = i;
			if (r == -1) return 0;
			pivot(r, s);
		}
	}
	ld solve(vector<ld> &x) {
		int r = 0;
		rep(i,1,m) if (D[i][n + 1] < D[r][n + 1]) r = i;
		if (D[r][n + 1] < -EPS) {
			pivot(r, n);
			if (!simplex(1) || D[m + 1][n + 1] < -EPS) return -INF;
			rep(i,0,m) if (B[i] == -1) {
				int s = -1;
				rep(j,0,n+1) if (s == -1 || D[i][j] < D[i][s] - EPS ||
					(abs(D[i][j] - D[i][s]) <= EPS && N[j] < N[s])) s = j;
				pivot(i, s);
			}
		}
		if (!simplex(2)) return INF;
		x.assign(n, 0);
		rep(i,0,m) if (B[i] < n) x[B[i]] = D[i][n + 1];
		return D[m][n + 1];
	}
};
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
