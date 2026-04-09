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
bool cmpX(Pt a, Pt b) { return a.xx != b.xx ? a.xx < b.xx : a.yy < b.yy; }

pair<Pt,Pt> closestPair(vector<Pt> p) {
	sort(all(p), cmpX);
	set<pair<ll,ll>> s;
	pair<Pt,Pt> ret{p[0], p[1]};
	ll best = norm(p[0] - p[1]);
	for (int l = 0, i = 0; i < sz(p); i++) {
		ll d = sqrtl((ld)best) + 1;
		while (p[i].xx - p[l].xx > d) s.erase({p[l].yy, p[l].xx}), l++;
		auto it1 = s.lower_bound({p[i].yy - d, LLONG_MIN});
		auto it2 = s.upper_bound({p[i].yy + d, LLONG_MAX});
		for (auto it = it1; it != it2; ++it) {
			Pt q(it->se, it->fi);
			if (ll cur = norm(p[i] - q); cur < best) best = cur, ret = {p[i], q};
		}
		s.insert({p[i].yy, p[i].xx});
	}
	return ret;
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
		auto [pt1, pt2] = closestPair(pt);
		int i = mp[{pt1.xx, pt1.yy}], j = mp[{pt2.xx, pt2.yy}];
		cout << i << ' ' << j << endl;
	}
}
