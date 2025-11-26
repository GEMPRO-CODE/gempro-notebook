#include <vector>
#include <complex>
#include <random>
#include <algorithm>
#include <iostream>
#include <cassert>

using namespace std;

using i64 = long long;

using VI = vector<int>;

using D = i64;
using point = complex<D>;

D dot(point a, point b) {
	return real(conj(a) * b);
}
D cross(point a, point b) {
	return imag(conj(a) * b);
}

// begin template //
bool pointInPoly(vector<point> &pt, point q, bool border = true) {
	int n = pt.size(), t = border, l = 1, r = n - 1;
	auto v = [&](int i) { return cross(q - pt[0], pt[i] - pt[0]); };
	if (v(l) >= t || v(r) <= -t) return 0;
	while (r - l > 1) {
		int m = (l + r) / 2;
		(v(m) > 0 ? r : l) = m;
	}
	return cross(pt[r] - pt[l], pt[l] - q) < t;
}
// end template //

// Test at https://codeforces.com/problemsets/acmsguru/problem/99999/253

istream &operator>>(istream &in, point &pt) {
	int x, y;
	cin >> x >> y;
	pt = point(x, y);
	return in;
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n, m, k;
	cin >> n >> m >> k;
	vector<point> pt(n);
	for (int i = 0; i < n; i++) cin >> pt[i];
	for (int i = 0; i < m; i++) {
		point a;
		cin >> a;
		if (pointInPoly(pt, a)) {
			k--;
		}
	}
	cout << (k <= 0 ? "YES" : "NO") << endl;
	return 0;
}
