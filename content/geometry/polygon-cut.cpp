#include <vector>
#include <random>
#include <complex>

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

Pt lineInter(Pt a, Pt b, Pt c, Pt d) {
	return a + (b - a) * cross(c - a, d - c) / cross(b - a, d - c);
}
// begin template //
vector<Pt> polyCut(vector<Pt> p, Pt a, Pt b) {
	vector<Pt> q;
	rep(i,0,sz(p)) {
		Pt c = p[i], d = p[(i + 1) % sz(p)];
		int sc = sgn(cross(b - a, c - a)), sd = sgn(cross(b - a, d - a));
		if (sc >= 0) q.pb(c);
		if (sc * sd < 0) q.pb(lineInter(a, b, c, d));
	}
	return q;
}
// end template //
