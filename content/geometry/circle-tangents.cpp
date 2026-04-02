#include <vector>
#include <random>
#include <complex>
#include <cmath>
#include <utility>

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

using Pt = complex<ld>;
#define xx real()
#define yy imag()

ld dot(Pt a, Pt b) { return (conj(a) * b).xx; }
ld cross(Pt a, Pt b) { return (conj(a) * b).yy; }
Pt perp(Pt a) { return Pt(-a.yy, a.xx); }
const ld EPS = 1e-9;
int sgn(ld x) { return (x > EPS) - (x < -EPS); }

// begin template //
vector<pair<Pt, Pt>> circTangents(Pt c1, ld r1, Pt c2, ld r2) {
	vector<pair<Pt, Pt>> res;
	for (int s: {1, -1}) {
		ld r = s * r2, dr = r1 - r;
		Pt d = c2 - c1;
		ld l2 = norm(d), h2 = max((ld)0, l2 - dr * dr);
		if (sgn(l2 - dr * dr) < 0) continue;
		for (int t: {1, -1}) {
			Pt n = (dr * d + perp(d) * (t * sqrt(h2))) / l2;
			res.pb({c1 + r1 * n, c2 + r * n});
		}
	}
	return res;
}
// end template //
