#include <vector>
#include <complex>

using namespace std;

using D = double;
using point = complex<D>;

D cross(point a, point b) { return imag(conj(a) * b); }

point lineIntersect(point a1, point d1, point a2, point d2) {
	return a1 + cross(a2 - a1, d2) / cross(d1, d2) * d1;
}

// begin template //
// D = double;
// Sutherland-Hodgman clip: keep the left side of directed line a→b.
// Clip poly by half-plane; chain calls for convex clipper.
vector<point> clipPoly(vector<point> &poly, point a, point b) {
	int n = poly.size();
	vector<point> res;
	for (int i = 0; i < n; i++) {
		point c = poly[i], d = poly[(i + 1) % n];
		D cc = cross(b - a, c - a), dc = cross(b - a, d - a);
		if (cc >= 0) res.push_back(c);
		if ((cc > 0) != (dc > 0)) res.push_back(lineIntersect(a, b - a, c, d - c));
	}
	return res;
}
// end template //
