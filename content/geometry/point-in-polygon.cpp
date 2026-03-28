#include <algorithm>
#include <iostream>
#include <cassert>
#include <vector>
#include <random>
#include <complex>

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

using Pt = complex<ll>;
#define xx real()
#define yy imag()

ll dot(Pt a, Pt b) { return (conj(a) * b).xx; }
ll cross(Pt a, Pt b) { return (conj(a) * b).yy; }
Pt perp(Pt a) { return Pt(-a.yy, a.xx); }

// begin template //
bool pointInPoly(vector<Pt> &pt, Pt q, bool border = true) {
	int n = pt.size(), t = border, l = 1, r = n - 1;
	auto v = [&](int i) { return cross(q - pt[0], pt[i] - pt[0]); };
	if (v(l) >= t || v(r) <= -t) return 0;
	while (r - l > 1) {
		int m = (l + r) / 2;
		(v(m) > 0 ? r : l) = m;
	}
	return cross(pt[r] - pt[l], pt[l] - q) < t;
}
// end template //

// Test at https://codeforces.com/problemsets/acmsguru/problem/99999/253

istream &operator>>(istream &in, Pt &pt) {
	int x, y;
	cin >> x >> y;
	pt = Pt(x, y);
	return in;
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n, m, k;
	cin >> n >> m >> k;
	vector<Pt> pt(n);
	for (int i = 0; i < n; i++) cin >> pt[i];
	for (int i = 0; i < m; i++) {
		Pt a;
		cin >> a;
		if (pointInPoly(pt, a)) {
			k--;
		}
	}
	cout << (k <= 0 ? "YES" : "NO") << endl;
	return 0;
}
