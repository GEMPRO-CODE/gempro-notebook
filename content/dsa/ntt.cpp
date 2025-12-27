#include <vector>
#include <cstdint>
#include <iostream>

using namespace std;

using i64 = int64_t;

using VI = vector<int>;

// Adapted from kactl

// begin template //
using VL = vector<i64>;

const int MOD = 998244353, root = 62;

i64 powm(i64 x, i64 e, i64 mod) {
	i64 r = 1;
	while (e) {
		if (e & 1) (r *= x) %= mod;
		e >>= 1;
		(x *= x) %= mod;
	}
	return r;
}

void ntt(VL &a) {
	int n = a.size(), L = 31 - __builtin_clz(n);
	static VL rt(2, 1);
	for (static int k = 2, s = 2; k < n; k *= 2, s++) {
		rt.resize(n);
		i64 z[] = {1, powm(root, MOD >> s, MOD)};
		for (int i = k; i < 2 * k; i++) rt[i] = rt[i / 2] * z[i & 1] % MOD;
	}
	VI rev(n);
	for (int i = 0; i < n; i++) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
	for (int i = 0; i < n; i++) if (i < rev[i]) swap(a[i], a[rev[i]]);
	for (int k = 1; k < n; k <<= 1)
		for (int i = 0; i < n; i += k << 1)
			for (int j = 0; j < k; j++) {
				i64 z = rt[j + k] * a[i + j + k] % MOD, &ai = a[i + j];
				a[i + j + k] = ai - z + (z > ai ? MOD : 0);
				ai += (ai + z >= MOD ? z - MOD : z);
			}
}
VL conv(VL &a, VL &b) {
	if (a.empty() || b.empty()) return {};
	int s = ssize(a) + ssize(b) - 1, B = 32 - __builtin_clz(s),
	    n = 1 << B;
	int inv = powm(n, MOD - 2, MOD);
	VL L(a), R(b), out(n);
	L.resize(n), R.resize(n);
	ntt(L), ntt(R);
	for (int i = 0; i < n; ++i)
		out[-i & (n - 1)] = (i64)L[i] * R[i] % MOD * inv % MOD;
	ntt(out);
	return {out.begin(), out.begin() + s};
}
// end template //

// Test at https://judge.yosupo.jp/problem/convolution_mod

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n, m;
	cin >> n >> m;
	vector<i64> a(n), b(m);
	for (int i = 0; i < n; i++) {
		cin >> a[i];
	}
	for (int i = 0; i < m; i++) {
		cin >> b[i];
	}
	auto c = conv(a, b);
	for (int x: c) {
		cout << x << ' ';
	}
	cout << endl;
	return 0;
}
