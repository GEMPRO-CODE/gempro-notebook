#include <utility>
using namespace std;
using i64 = long long;

i64 egcd(i64 a, i64 b, i64 &x, i64 &y) {
	if (b == 0) {
		x = 1, y = 0;
		return a;
	}
	i64 g = egcd(b, a % b, y, x);
	y -= (a / b) * x;
	return g;
}

// begin template //
// x = r1 (mod m1), x = r2 (mod m2) -> x = r (mod m)
// returns {r, m} or {0, -1} if incompatible
// chain calls to combine more congruences
pair<i64, i64> crt(i64 r1, i64 m1, i64 r2, i64 m2) {
	i64 x, y, g = egcd(m1, m2, x, y);
	if ((r2 - r1) % g) return {0, -1};
	i64 k = m2 / g, m = m1 / g * m2;
	i64 q = ((r2 - r1) / g % k + k) % k;
	x = ((x % k) + k) % k;
	i64 r = r1 + (__int128)m1 * (q * x % k);
	return {(r % m + m) % m, m};
}
// end template //

// Test at https://judge.yosupo.jp/problem/garner

#include <iostream>
using namespace std;
int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n; cin >> n;
	i64 r = 0, m = 1;
	for (int i = 0; i < n; i++) {
		i64 ri, mi; cin >> ri >> mi;
		auto [nr, nm] = crt(r, m, ri, mi);
		if (nm == -1) { cout << -1 << "\n"; return 0; }
		r = nr; m = nm;
	}
	cout << r << "\n";
}
