#include <algorithm>
#include <complex>
#include <numbers>
#include <vector>


using namespace std;

using VI = vector<int>;

// Adapted from kactl

// begin template //
using ld = double;
const long double PI = numbers::pi;
using VD = vector<ld>;
using C = complex<ld>;

void fft(vector<C> &a) {
	int n = a.size(), L = 31 - __builtin_clz(n);
	static vector<complex<long double>> R(2, 1);
	static vector<C> rt(2, 1);
	for (static int k = 2; k < n; k <<= 1) {
		R.resize(n); rt.resize(n);
		auto x = polar(1.0L, PI / k);
		for (int i = k; i < 2 * k; i++)
			rt[i] = R[i] = i & 1 ? R[i / 2] * x : R[i / 2];
	}
	VI rev(n);
	for (int i = 0; i < n; i++) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
	for (int i = 0; i < n; i++) if (i < rev[i]) swap(a[i], a[rev[i]]);
	for (int k = 1; k < n; k <<= 1)
		for (int i = 0; i < n; i += k << 1)
			for (int j = 0; j < k; j++) {
				auto x = (ld *)&rt[j + k], y = (ld *)&a[i + j + k];
				C z(x[0] * y[0] - x[1] * y[1], x[0] * y[1] + x[1] * y[0]);
				a[i + j + k] = a[i + j] - z;
				a[i + j] += z;
			}
}

VD conv(VD &a, VD &b) {
	if (a.empty() || b.empty())
		return {};
	VD res(ssize(a) + ssize(b) - 1);
	int L = 32 - __builtin_clz(res.size()), n = 1 << L;
	vector<C> in(n), out(n);
	copy(a.begin(), a.end(), in.begin());
	for (int i = 0; i < b.size(); i++) in[i].imag(b[i]);
	fft(in);
	for (C &x : in) x *= x;
	for (int i = 0; i < n; i++) out[i] = in[-i & (n - 1)] - conj(in[i]);
	fft(out);
	for (int i = 0; i < res.size(); i++) res[i] = imag(out[i]) / (4 * n);
	return res;
}
// end template //
