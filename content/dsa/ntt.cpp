#include <vector>
#include <cstdint>
#include <iostream>
#include <array>
#include <span>

using namespace std;

using i64 = long long;

using VI = vector<int>;

// begin template //
using VL = vector<i64>;

const int MOD = 998244353, root = 62;

i64 powm(i64 x, i64 e) {
	i64 r = 1;
	while (e) {
		if (e & 1) (r *= x) %= MOD;
		e >>= 1;
		(x *= x) %= MOD;
	}
	return r;
}

void ntt(VL &a) {
	int n = a.size(), k = bit_width(unsigned(n)) - 1;
	static array<i64, 1 << 23> r = {1, 1};
	for (static int t = 2, s = 2; t < n; t <<= 1, s++) {
		array z = {1ll, powm(root, MOD >> s)};
		for (int i = t; i < t << 1; i++) r[i] = r[i >> 1] * z[i & 1] % MOD;
	}
	static array<i64, 1 << 23> b, c;
	copy(a.begin(), a.end(), b.begin());
	for (int m = n; m >>= 1;) {
		for (int l = 0; l < n; l += m << 1) 
			for (int i = 0; i < m; i++) 
				c[l + i] = b[l + 2 * i], c[l + m + i] = b[l + 2 * i + 1];
		copy(c.begin(), c.begin() + n, b.begin());
	}
	for (int m = 1; m < n; m <<= 1) {
		for (int l = 0; l < n; l += m << 1)
			for (int i = 0; i < m; i++) {
				i64 z = r[m + i] * b[l + m + i] % MOD;
				c[l + i] = b[l + i] + z;
				c[l + m + i] = b[l + i] + (MOD - z);
			}
		for (int i = 0; i < n; i++) b[i] = c[i] < MOD ? c[i] : c[i] - MOD;
	}
	copy(b.begin(), b.begin() + n, a.begin());
}

VL conv(VL &a, VL &b) {
	if (a.empty() || b.empty()) return {};
	int s = ssize(a) + ssize(b) - 1, B = 32 - __builtin_clz(s), n = 1 << B;
	int inv = powm(n, MOD - 2);
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
