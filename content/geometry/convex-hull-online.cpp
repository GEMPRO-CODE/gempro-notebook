#include <complex>
#include <map>

using namespace std;

using D = double;
using point = complex<D>;

// begin template //
// D = double;
// Online convex hull: insert points one by one, query containment.
// add(p): insert point p. has(p): true if p is inside or on the hull.
struct ConvHullDyn {
	using M = map<D, D>;
	M up, lo;
	bool bad(M &h, M::iterator y) {
		if (y == h.begin() || next(y) == h.end()) return false;
		auto a = prev(y), b = next(y);
		return (b->first - a->first) * (y->second - a->second) <=
		       (b->second - a->second) * (y->first - a->first);
	}
	void ins(M &h, D x, D y) {
		if (h.count(x) && h[x] >= y) return;
		h[x] = y;
		auto it = h.find(x);
		if (bad(h, it)) { h.erase(it); return; }
		while (it != h.begin() && bad(h, prev(it))) h.erase(prev(it));
		while (next(it) != h.end() && bad(h, next(it))) h.erase(next(it));
	}
	void add(point p) { ins(up, real(p), imag(p)); ins(lo, real(p), -imag(p)); }
	bool has(point p) {
		if (up.size() < 2) return false;
		auto it = up.lower_bound(real(p));
		auto jt = lo.lower_bound(real(p));
		if (it == up.end() || it == up.begin()) return false;
		if (jt == lo.end() || jt == lo.begin()) return false;
		auto bU = it, aU = prev(it), bL = jt, aL = prev(jt);
		D yu = aU->second + (bU->second - aU->second) * (real(p) - aU->first) / (bU->first - aU->first);
		D yl = -(aL->second + (bL->second - aL->second) * (real(p) - aL->first) / (bL->first - aL->first));
		return imag(p) <= yu + 1e-9 && imag(p) >= yl - 1e-9;
	}
};
// end template //
