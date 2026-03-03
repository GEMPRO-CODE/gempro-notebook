#include <vector>
#include <complex>
#include <algorithm>

using namespace std;

using i64 = long long;
using D = i64;
using point = complex<D>;

D cross(point a, point b) { return imag(conj(a) * b); }

// begin template //
// Minkowski sum of convex polygons a and b (both CCW).
// Result is also CCW.
vector<point> minkowski(vector<point> a, vector<point> b) {
	auto cmp = [](point p, point q) {
		return make_pair(imag(p), real(p)) < make_pair(imag(q), real(q));
	};
	rotate(a.begin(), min_element(a.begin(), a.end(), cmp), a.end());
	rotate(b.begin(), min_element(b.begin(), b.end(), cmp), b.end());
	int n = a.size(), m = b.size();
	a.push_back(a[0]); a.push_back(a[1]);
	b.push_back(b[0]); b.push_back(b[1]);
	vector<point> res;
	for (int i = 0, j = 0; i < n || j < m; ) {
		res.push_back(a[i] + b[j]);
		D c = cross(a[i+1] - a[i], b[j+1] - b[j]);
		i += (c >= 0); j += (c <= 0);
	}
	return res;
}
// end template //

// Test at https://judge.yosupo.jp/problem/minkowski_sum_of_convex_polygons

#include <iostream>
istream &operator>>(istream &in, point &p) {
	i64 x, y; in >> x >> y; p = point(x, y); return in;
}
int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n, m; cin >> n >> m;
	vector<point> a(n), b(m);
	for (auto &p: a) cin >> p;
	for (auto &p: b) cin >> p;
	auto res = minkowski(a, b);
	cout << res.size() << "\n";
	for (auto p: res) cout << real(p) << " " << imag(p) << "\n";
}
