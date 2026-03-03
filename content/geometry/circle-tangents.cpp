#include <vector>
#include <complex>
#include <cmath>
#include <utility>

using namespace std;

using D = double;
using point = complex<D>;

// begin template //
// D = double;
// Common tangents of circles (c1,r1) and (c2,r2).
// Returns tangent contact pairs {p on c1, p on c2}, up to 4.
// s=1: external tangents (circles same side); s=-1: internal (opposite sides).
vector<pair<point, point>> circTangents(point c1, D r1, point c2, D r2) {
	vector<pair<point, point>> res;
	for (int s: {1, -1}) {
		D r = s * r2, dr = r1 - r;
		point d = c2 - c1;
		D l2 = norm(d), h2 = max((D)0, l2 - dr * dr);
		if (l2 < dr * dr - 1e-9) continue;
		for (int t: {1, -1}) {
			point n = (dr * d + point(-imag(d), real(d)) * (t * sqrt(h2))) / l2;
			res.push_back({c1 + r1 * n, c2 + r * n});
		}
	}
	return res;
}
// end template //
