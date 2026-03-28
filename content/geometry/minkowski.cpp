#include <algorithm>
#include <vector>
#include <random>
#include <complex>

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

using Pt = complex<ll>;
#define xx real()
#define yy imag()

ll dot(Pt a, Pt b) { return (conj(a) * b).xx; }
ll cross(Pt a, Pt b) { return (conj(a) * b).yy; }
Pt perp(Pt a) { return Pt(-a.yy, a.xx); }

// begin template //
bool ptLt(Pt a, Pt b) { return a.xx != b.xx ? a.xx < b.xx : a.yy < b.yy; }

vector<Pt> minkowski(vector<Pt> a, vector<Pt> b) {
	auto norm = [&](vector<Pt>& p) {
		rotate(p.begin(), min_element(all(p), ptLt), p.end());
		p.pb(p[0]), p.pb(p[1]);
	};
	int n = sz(a), m = sz(b);
	norm(a), norm(b);
	vector<Pt> c;
	for (int i = 0, j = 0; i < n || j < m; ) {
		c.pb(a[i] + b[j]);
		ll z = cross(a[i + 1] - a[i], b[j + 1] - b[j]);
		if (z >= 0 && i < n) i++;
		if (z <= 0 && j < m) j++;
	}
	return c;
}
// end template //

// Test at https://judge.yosupo.jp/problem/minkowski_sum_of_convex_polygons

#include <iostream>
istream &operator>>(istream &in, Pt &p) {
	ll x, y; in >> x >> y; p = Pt(x, y); return in;
}
int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n, m; cin >> n >> m;
	vector<Pt> a(n), b(m);
	for (auto &p: a) cin >> p;
	for (auto &p: b) cin >> p;
	auto res = minkowski(a, b);
	cout << res.size() << "\n";
	for (auto p: res) cout << real(p) << " " << imag(p) << "\n";
}
