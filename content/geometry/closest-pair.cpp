#include <vector>
#include <complex>
#include <algorithm>
#include <set>
#include <map>
#include <vector>
#include <random>
#include <complex>
#include <iostream>
#include <climits>

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
const ld EPS = 1e-9;
int sgn(ld x) { return (x > EPS) - (x < -EPS); }
// begin template //
ll dist2(Pt a, Pt b) { // ld: ll -> ld
	ll dx = a.xx - b.xx, dy = a.yy - b.yy; // ld: ll -> ld
	return dx * dx + dy * dy;
}

pii closestPair(vector<Pt> p) {
	vector<pair<Pt,int>> a;
	rep(i,0,sz(p)) a.eb(p[i], i);
	sort(all(a), [](auto a, auto b) {
		return a.fi.xx != b.fi.xx ? a.fi.xx < b.fi.xx : a.fi.yy < b.fi.yy;
	});
	ll ans = dist2(a[0].fi, a[1].fi), l = 0;
	pii best = {a[0].se, a[1].se};
	set<tuple<ll,ll,int>> s; // ld: use tuple<ld, ld, int>
	s.insert({a[0].fi.yy, a[0].fi.xx, a[0].se});
	s.insert({a[1].fi.yy, a[1].fi.xx, a[1].se});
	rep (i, 2, sz(a)) {
		ll d = (ll)sqrtl((ld)ans) + 1; // ld: ld d = sqrtl(ans) + EPS
		while (a[i].fi.xx - a[l].fi.xx > d) {
			s.erase({a[l].fi.yy, a[l].fi.xx, a[l].se});
			l++;
		}
		auto it1 = s.lower_bound({a[i].fi.yy - d, LLONG_MIN, INT_MIN});
		auto it2 = s.upper_bound({a[i].fi.yy + d, LLONG_MAX, INT_MAX});
		for (auto it = it1; it != it2; ++it) {
			int j = get<2>(*it);
			ll cur = dist2(a[i].fi, p[j]); // ld: ld cur = ...
			if (cur < ans) ans = cur, best = {a[i].se, j};
		}
		s.insert({a[i].fi.yy, a[i].fi.xx, a[i].se});
	}
	return best;
}
// end template //

// Test at https://judge.yosupo.jp/problem/closest_pair

istream &operator>>(istream &in, Pt &pt) {
	ll x, y;
	in >> x >> y;
	pt = Pt{x, y};
	return in;
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int t;
	cin >> t;
	while (t--) {
		int n;
		cin >> n;
		vector<Pt> pt(n);
		rep(i, 0, n) cin >> pt[i];
		map<pii, int> mp;
		bool done = 0;
		rep(i, 0, n) {
			if (mp.count(pair(pt[i].xx, pt[i].yy))) {
				cout << mp[pair(pt[i].xx, pt[i].yy)] << ' ' << i << endl;
				done = 1;
				break;
			}
			mp[pair(pt[i].xx, pt[i].yy)] = i;
		}
		if (done) continue;
		auto [i, j] = closestPair(pt);
		cout << i << ' ' << j << endl;
	}
}
