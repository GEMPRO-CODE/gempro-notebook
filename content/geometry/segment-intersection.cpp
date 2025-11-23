#include <complex>
#include <array>
#include <iostream>

using namespace std;

using i64 = long long;

using D = i64;
using point = complex<D>;

D dot(point a, point b) {
	return real(conj(a) * b);
}
D cross(point a, point b) {
	return imag(conj(a) * b);
}

// begin template //
#define CK(a, b, t) max(t(a[0]), t(a[1])) < min(t(b[0]), t(b[1]))
#define CKK(a, b, t) (s(a[0], a[1], b[t]) == 0 && dot(b[t] - a[0], b[t] - a[1]) <= 0)

bool segInterChk(point a, point b, point c, point d) {
	array p = {a, b}, q = {c, d};
	auto s = [](point p, point q, point r) { return cross(q - p, r - p); };
	if (CK(p, q, real) || CK(p, q, imag) || CK(q, p, real) || CK(q, p, imag)) return false;
	if (CKK(p, q, 0) || CKK(p, q, 1) || CKK(q, p, 0) || CKK(q, p, 1)) return true;
	return (s(a, b, c) > 0) != (s(a, b, d) > 0) && (s(c, d, a) > 0) != (s(c, d, b) > 0);
}
// end template //

// Test at https://cses.fi/problemset/task/2190/

istream &operator>>(istream &in, point &s) {
	int x, y;
	in >> x >> y;
	s = point(x, y);
	return in;
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int t;
	cin >> t;
	while (t--) {
		point a, b, c, d;
		cin >> a >> b >> c >> d;
		cout << (segInterChk(a, b, c, d) ? "YES": "NO") << endl;
	}
	return 0;
}
