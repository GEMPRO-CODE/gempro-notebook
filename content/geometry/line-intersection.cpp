#include <complex>

using namespace std;

using i64 = long long;

using D = i64;
using point = complex<D>;

D dot(point a, point b) {
	return real(conj(a) * b);
}
D cross(point a, point b) {
	return imag(conj(a) * b);
}

// begin template //
// D = double;

point lineIntersect(point a1, point d1, point a2, point d2) {
	return a1 + cross(a2 - a1, d2) / cross(d1, d2) * d1;
}
// end template //
