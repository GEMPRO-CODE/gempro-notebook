#include <algorithm>
#include <cmath>
#include <complex>
#include <vector>

using namespace std;
using ld = long double;
using Pt = complex<ld>;

ld dot(Pt a, Pt b) { return (conj(a) * b).real(); }
ld cross(Pt a, Pt b) { return (conj(a) * b).imag(); }

// begin template //
ld circlePolyArea(const vector<Pt> &p, Pt c, ld r) {
	if (r <= 0 || p.size() < 3) return 0;
	ld ans = 0, r2 = r * r;
	for (int i = 0, n = p.size(); i < n; i++) {
		Pt a = p[i] - c, b = p[(i + 1) % n] - c, d = b - a;
		ld len2 = norm(d);
		if (!len2) continue;
		vector<ld> ts = {0, 1};
		ld t = -dot(a, d) / len2, h2 = r2 - norm(a + t * d);
		if (h2 > 0) {
			ld dt = sqrt(h2 / len2);
			for (ld x : {t - dt, t + dt})
				if (0 < x && x < 1) ts.push_back(x);
		}
		sort(ts.begin(), ts.end());
		for (int j = 1; j < (int)ts.size(); j++) {
			Pt u = a + ts[j - 1] * d, v = a + ts[j] * d;
			if (norm((u + v) / (ld)2) < r2) ans += cross(u, v);
			else ans += r2 * atan2(cross(u, v), dot(u, v));
		}
	}
	return abs(ans) / 2;
}
// end template //
