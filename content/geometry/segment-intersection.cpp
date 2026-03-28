#include <vector>
#include <random>
#include <complex>
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
int sgn(ld x) { return (x > EPS) - (x < -EPS); } // change EPS to 0 when using ll
// begin template //

bool segInter(Pt a, Pt b, Pt c, Pt d) {
	int ab_c = sgn(cross(b - a, c - a)), ab_d = sgn(cross(b - a, d - a));
	int cd_a = sgn(cross(d - c, a - c)), cd_b = sgn(cross(d - c, b - c));
	if (ab_c * ab_d < 0 && cd_a * cd_b < 0) return 1;
	if (!ab_c && sgn(dot(c - a, c - b)) <= 0) return 1;
	if (!ab_d && sgn(dot(d - a, d - b)) <= 0) return 1;
	if (!cd_a && sgn(dot(a - c, a - d)) <= 0) return 1;
	if (!cd_b && sgn(dot(b - c, b - d)) <= 0) return 1;
	return 0;
}

bool properInter(Pt a, Pt b, Pt c, Pt d) {
	int ab_c = sgn(cross(b - a, c - a)), ab_d = sgn(cross(b - a, d - a));
	int cd_a = sgn(cross(d - c, a - c)), cd_b = sgn(cross(d - c, b - c));
	return ab_c * ab_d < 0 && cd_a * cd_b < 0;
}
// end template //

// Test at https://cses.fi/problemset/task/2190/

istream &operator>>(istream &in, Pt &s) {
	int x, y;
	in >> x >> y;
	s = Pt(x, y);
	return in;
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int t;
	cin >> t;
	while (t--) {
		Pt a, b, c, d;
		cin >> a >> b >> c >> d;
		cout << (segInter(a, b, c, d) ? "YES": "NO") << endl;
	}
	return 0;
}
