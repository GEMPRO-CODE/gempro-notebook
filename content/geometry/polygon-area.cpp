#include <iostream>
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
ll polyArea(vector<Pt> &pt) { // ld: change ret type to ld
	ll n = sz(pt), s = 0; // ld: change to ld s 
	rep (i, 0, n) s += cross(pt[i], pt[(i + 1) % n]);
	return s;
}
// end template //

// Test at https://cses.fi/problemset/task/2191/

istream &operator>>(istream &in, Pt &pt) {
	ll x, y;
	in >> x >> y;
	pt = Pt(x, y);
	return in;
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n;
	cin >> n;
	vector<Pt> pt(n);
	for (int i = 0; i < n; i++) {
		cin >> pt[i];
	}
	cout << llabs(polyArea(pt)) << endl;
	return 0;
}
