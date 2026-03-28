#include <algorithm>
#include <cmath>
#include <complex>
#include <random>
#include <vector>

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

// Adapted from kactl

// begin template //
using cd = complex<ld>;
const ld PI = acosl(-1.0L);

void fft(vector<cd> &a) {
	int n = sz(a), lg = 31 - __builtin_clz(n);
	static vector<cd> rt(2, 1);
	for (static int k = 2; k < n; k <<= 1) {
		rt.resize(n);
		cd x = polar((ld)1, PI / k);
		rep(i, k, 2 * k) rt[i] = i & 1 ? rt[i / 2] * x : rt[i / 2];
	}
	vi rev(n);
	rep(i, 0, n) rev[i] = (rev[i / 2] | ((i & 1) << lg)) / 2;
	rep(i, 0, n) if (i < rev[i]) swap(a[i], a[rev[i]]);
	for (int k = 1; k < n; k <<= 1)
		for (int i = 0; i < n; i += k << 1)
			rep(j, 0, k) {
				cd z = rt[j + k] * a[i + j + k];
				a[i + j + k] = a[i + j] - z;
				a[i + j] += z;
			}
}

vector<ld> conv(vector<ld> &a, vector<ld> &b) {
	if (a.empty() || b.empty()) return {};
	int s = sz(a) + sz(b) - 1, n = 1;
	while (n < s) n <<= 1;
	vector<cd> in(n), out(n);
	rep(i, 0, sz(a)) in[i].real(a[i]);
	rep(i, 0, sz(b)) in[i].imag(b[i]);
	fft(in);
	for (cd &x : in) x *= x;
	rep(i, 0, n) out[i] = in[-i & (n - 1)] - conj(in[i]);
	fft(out);
	vector<ld> res(s);
	rep(i, 0, s) res[i] = imag(out[i]) / (4 * n);
	return res;
}
// end template //
