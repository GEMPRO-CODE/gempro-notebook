#include <algorithm>
#include <cassert>
#include <cmath>
#include <utility>
using namespace std;
using ld = long double;

// begin template //
template<class F>
pair<ld,bool> integrate(F f, ld a, ld b, ld eps = 1e-9L,
		int depth = 25, int budget = 100000) {
	assert(eps > 0 && depth >= 0 && budget >= 3);
	if (a == b) return {0,true};
	ld sign = 1; if (a > b) swap(a,b), sign = -1;
	auto simpson = [](ld a, ld b, ld fa, ld fm, ld fb) {
		return (b-a)*(fa+4*fm+fb)/6;
	};
	bool ok = true;
	auto rec = [&](auto&& self, ld a, ld b, ld fa, ld fm, ld fb,
			ld s, ld tol, int dep) -> ld {
		ld m = a+(b-a)/2, l = a+(m-a)/2, r = m+(b-m)/2;
		if (!dep || budget < 2 || l == a || r == b) {
			ok = false; return s;
		}
		budget -= 2;
		ld fl = f(l), fr = f(r);
		ld sl = simpson(a,m,fa,fl,fm), sr = simpson(m,b,fm,fr,fb);
		ld delta = sl+sr-s;
		if (abs(delta) <= 15*tol) return sl+sr+delta/15;
		ld left = self(self,a,m,fa,fl,fm,sl,tol/2,dep-1);
		return left+self(self,m,b,fm,fr,fb,sr,tol/2,dep-1);
	};
	ld fa = f(a), fm = f(a+(b-a)/2), fb = f(b); budget -= 3;
	ld ans = rec(rec,a,b,fa,fm,fb,simpson(a,b,fa,fm,fb),eps,depth);
	return {sign*ans,ok};
}
// end template //
