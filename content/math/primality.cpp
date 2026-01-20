#include <iostream>

using namespace std;

using i64 = long long;
using i128 = __int128;

// begin template //
i64 powm(i64 x, i64 e, i64 mod) {
	i64 r = 1;
	while (e) {
		if (e & 1) r = i128(r)* x % mod;
		e >>= 1;
		x = i128(x) * x % mod;
	}
	return r;
}

bool checkPrime(i64 p, i64 a) {
	i64 k = p - 1, d = 1;
	while (~k & 1) k >>= 1;
	for (i64 e = k; e; e >>= 1) {
		if (e & 1) d = i128(d) * a % p;
		a = i128(a) * a % p;
	}
	if (d == 1 || d == p - 1) return true;
	while ((k <<= 1) < p - 1) {
		d = i128(d) * d % p;
		if (d == p - 1) return true;
	}
	return false;
}

bool isPrime(i64 p) {
	if (p == 1) return false;
	for (int i: {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
		if (p == i) return true;
		if (!checkPrime(p, i)) return false;
	}
	return true;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int q;
	cin >> q;
	while (q--) {
		i64 n;
		cin >> n;
		if (isPrime(n)) {
			cout << "Yes" << endl;
		} else {
			cout << "No" << endl;
		}
	}
	return 0;
}
