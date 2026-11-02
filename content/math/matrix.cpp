#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numeric>
#include <random>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
using ll = long long;
using ld = long double;
using vi = vector<int>;
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)

// begin template //
template<class T> using Matrix = vector<vector<T>>;
template<class T, class Add, class Mul>
Matrix<T> matMul(const Matrix<T> &a, const Matrix<T> &b,
	T zero, Add add, Mul mul) {
	int n = sz(a), k = sz(b), m = sz(b[0]);
	Matrix<T> c(n,vector<T>(m,zero));
	rep(i,0,n) rep(j,0,k) rep(t,0,m)
		c[i][t] = add(c[i][t],mul(a[i][j],b[j][t]));
	return c;
}
template<class T, class Add, class Mul>
Matrix<T> matPow(Matrix<T> a, uint64_t e,
	T zero, T one, Add add, Mul mul) {
	int n = sz(a);
	Matrix<T> r(n,vector<T>(n,zero));
	rep(i,0,n) r[i][i] = one;
	while (e) {
		if (e&1) r = matMul(r,a,zero,add,mul);
		e >>= 1;
		if (e) a = matMul(a,a,zero,add,mul);
	}
	return r;
}
// end template //
