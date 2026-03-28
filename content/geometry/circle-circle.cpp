#include <vector>
#include <random>
#include <complex>
#include <cmath>

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
vector<Pt> circCircInter(Pt c1, ld r1, Pt c2, ld r2) {
	Pt d = c2 - c1;
	ld dist = abs(d);
	if (!sgn(dist)) return {};
	ld a = (norm(d) + r1 * r1 - r2 * r2) / (2 * dist);
	ld h2 = r1 * r1 - a * a;
	if (sgn(h2) < 0) return {};
	Pt mid = c1 + d * (a / dist);
	if (!sgn(h2)) return {mid};
	Pt p = perp(d) * (sqrt(max((ld)0, h2)) / dist);
	return {mid - p, mid + p};
}
// end template //
