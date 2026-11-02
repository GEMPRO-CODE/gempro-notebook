// https://cp-algorithms.com/geometry/halfplane-intersection.html
#include <algorithm>
#include <cmath>
#include <complex>
#include <vector>
using namespace std;
using ld = long double;
#define sz(x) (int)(x).size()
#define all(x) begin(x), end(x)
#define pb push_back
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
using Pt = complex<ld>;
ld cross(Pt a,Pt b) { return imag(conj(a)*b); }
ld dot(Pt a,Pt b) { return real(conj(a)*b); }
const ld EPS = 1e-9L;

// begin template //
struct Halfplane {
	Pt p, d; ld angle;
	Halfplane(Pt a, Pt b): p(a), d((b-a)/abs(b-a)), angle(arg(d)) {}
	bool out(Pt q) const { return cross(d,q-p) < -EPS; }
};
Pt hpInter(Halfplane a, Halfplane b) {
	return a.p + a.d * (cross(b.p-a.p,b.d)/cross(a.d,b.d));
}
vector<Pt> halfplaneInter(vector<Halfplane> h, ld bound) {
	vector<Pt> box = {{-bound,-bound},{bound,-bound},
		{bound,bound},{-bound,bound}};
	rep(i,0,4) h.emplace_back(box[i],box[(i+1)%4]);
	sort(all(h), [](auto a, auto b) { return a.angle < b.angle; });
	vector<Halfplane> q = h;
	int l = 0, r = 0;
	for (auto a : h) {
		while (r-l>1 && a.out(hpInter(q[r-2],q[r-1]))) --r;
		while (r-l>1 && a.out(hpInter(q[l],q[l+1]))) ++l;
		if (r>l && abs(cross(a.d,q[r-1].d)) < EPS) {
			if (dot(a.d,q[r-1].d) < 0) return {};
			if (!a.out(q[r-1].p)) continue;
			--r;
		}
		q[r++] = a;
	}
	while (r-l>2 && q[l].out(hpInter(q[r-2],q[r-1]))) --r;
	while (r-l>2 && q[r-1].out(hpInter(q[l],q[l+1]))) ++l;
	if (r-l<3) return {};
	vector<Pt> p;
	rep(i,l,r) p.pb(hpInter(q[i],q[i+1<r ? i+1 : l]));
	ld area = 0;
	rep(i,0,sz(p)) area += cross(p[i]-p[0],p[(i+1)%sz(p)]-p[0]);
	return area > EPS ? p : vector<Pt>{};
}
// end template //
