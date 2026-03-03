#include <vector>
#include <complex>
#include <cmath>

using namespace std;

using D = double;
using point = complex<D>;

// begin template //
// D = double;
// Returns 0, 1 or 2 intersection points of circles (c1,r1) and (c2,r2).
vector<point> circCircInter(point c1, D r1, point c2, D r2) {
	point d = c2 - c1;
	D dist = abs(d);
	if (dist < 1e-9) return {};
	D a = (norm(d) + r1 * r1 - r2 * r2) / (2 * dist);
	D h2 = r1 * r1 - a * a;
	if (h2 < -1e-9) return {};
	point mid = c1 + a / dist * d;
	if (h2 < 1e-9) return {mid};
	point perp = point(-imag(d), real(d)) * (sqrt(max((D)0, h2)) / dist);
	return {mid - perp, mid + perp};
}
// end template //
