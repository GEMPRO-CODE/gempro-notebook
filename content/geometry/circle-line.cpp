#include <vector>
#include <complex>
#include <cmath>

using namespace std;

using D = double;
using point = complex<D>;

D dot(point a, point b) { return real(conj(a) * b); }
D cross(point a, point b) { return imag(conj(a) * b); }

// begin template //
// D = double;
// Circle center c, radius r. Line through a in direction d.
// Returns 0, 1 or 2 intersection points.
vector<point> circLineInter(point c, D r, point a, point d) {
	point f = a + dot(c - a, d) / dot(d, d) * d;
	D h2 = r * r - norm(c - f);
	if (h2 < -1e-9) return {};
	D dt = sqrt(max((D)0, h2)) / abs(d);
	if (h2 < 1e-9) return {f};
	return {f - dt * d, f + dt * d};
}
// end template //
