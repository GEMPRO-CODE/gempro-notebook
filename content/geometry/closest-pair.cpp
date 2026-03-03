#include <vector>
#include <complex>
#include <algorithm>
#include <set>
#include <iostream>

using namespace std;

using D = double;
using point = complex<D>;

// begin template //
// D = double;
// Minimum euclidean distance between any two points. O(n log n).
D closestPair(vector<point> pts) {
	sort(pts.begin(), pts.end(), [](point a, point b) {
		return make_pair(real(a), imag(a)) < make_pair(real(b), imag(b));
	});
	D best = abs(pts[0] - pts[1]);
	set<pair<D, D>> S;
	int l = 0;
	for (auto p: pts) {
		D x = real(p), y = imag(p);
		while (x - real(pts[l]) > best) S.erase({imag(pts[l]), real(pts[l++])});
		for (auto it = S.lower_bound({y - best, -1e18}); it != S.end() && it->first <= y + best; it++)
			best = min(best, abs(p - point(it->second, it->first)));
		S.insert({y, x});
	}
	return best;
}
// end template //

// Test at https://judge.yosupo.jp/problem/closest_pair

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n; cin >> n;
	vector<point> pts(n);
	for (auto &p: pts) { D x, y; cin >> x >> y; p = point(x, y); }
	cout << fixed;
	cout.precision(15);
	cout << closestPair(pts) << "\n";
}
