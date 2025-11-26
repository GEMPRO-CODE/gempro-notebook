#include <vector>
#include <complex>
#include <iostream>

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
D polyArea(vector<point> &pt) {
	int n = pt.size();
	D s = 0;
	for (int i = 2; i < n; i++) s += cross(pt[i - 1] - pt[0], pt[i] - pt[0]);
	return s;
}
// end template //

// Test at https://cses.fi/problemset/task/2191/

istream &operator>>(istream &in, point &pt) {
	i64 x, y;
	in >> x >> y;
	pt = point(x, y);
	return in;
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n;
	cin >> n;
	vector<point> pt(n);
	for (int i = 0; i < n; i++) {
		cin >> pt[i];
	}
	cout << llabs(polyArea(pt)) << endl;
	return 0;
}
