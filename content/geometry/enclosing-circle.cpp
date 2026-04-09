#include <vector>
#include <random>
#include <complex>
#include <algorithm>
#include <iostream>

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
struct C {
	Pt o;
	ld r;
};

bool in(C c, Pt p) { return abs(p - c.o) <= c.r + EPS; }

C circ(Pt a, Pt b) { return {(a + b) / (ld)2, abs(a - b) / 2}; }

C circ(Pt a, Pt b, Pt c) {
	if (fabsl(cross(b - a, c - a)) < EPS) {
		C x = circ(a, b), y = circ(a, c), z = circ(b, c);
		if (in(x, c)) return x;
		if (in(y, b)) return y;
		return z;
	}
	Pt o = a + perp((b - a) * norm(c - a) - (c - a) * norm(b - a))
		/ cross(b - a, c - a) / (ld)2;
	return {o, abs(a - o)};
}

C mec(vector<Pt> p) {
	shuffle(all(p), rng);
	C c{{0, 0}, -1};
	rep(i,0,sz(p)) if (!in(c, p[i])) {
		c = {p[i], 0};
		rep(j,0,i) if (!in(c, p[j])) {
			c = circ(p[i], p[j]);
			rep(k,0,j) if (!in(c, p[k])) c = circ(p[i], p[j], p[k]);
		}
	}
	return c;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
