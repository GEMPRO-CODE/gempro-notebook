#include <vector>
#include <complex>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <random>
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
vector<int> convHull(vector<Pt> &pt) {
	int n = sz(pt), m;
	vi h, ord(n);
	auto add = [&]() {
		vi st;
		for (int i: ord) {
			while ((m = sz(st)) > 1) {
				Pt a = pt[st[m - 1]], b = pt[st[m - 2]], c = pt[i];
				if (cross(b - a, c - a) < 0) break; // > for clockwise, <= to include non-vertices
				st.pop_back();
			}
			st.push_back(i);
		}
		st.pop_back();
		h.insert(h.end(), all(st));
	};
	iota(all(ord), 0);
	auto top = [](auto a) { return pair(a.xx, a.yy); };
	sort(all(ord), [&](int i, int j) { return top(pt[i]) > top(pt[j]); });
	add(), reverse(all(ord)), add();
	return h;
}
// end template //

// Test at https://judge.yosupo.jp/problem/static_convex_hull

istream &operator>>(istream &in, Pt &pt) {
	ll x, y;
	cin >> x >> y;
	pt = Pt(x, y);
	return in;
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int t;
	cin >> t;
	while (t--) {
		int n;
		cin >> n;
		if (n == 0) {
			cout << 0 << endl;
			continue;
		}
		vector<Pt> pt(n);
		for (int i = 0; i < n; i++) cin >> pt[i];
		auto ch = convHull(pt);
		if (ch.size() == 0) {
			cout << 1 << endl;
			cout << real(pt[0]) << ' ' << imag(pt[0]) << endl;
			continue;
		}
		vector<Pt> ans;
		Pt last(1e12, 1e12);
		for (int i: ch) {
			if (pt[i] == last) continue;
			ans.push_back(pt[i]);
			last = pt[i];
		}
		cout << ans.size() << endl;
		for (auto i: ans) cout << real(i) << ' ' << imag(i) << endl;
	}
	return 0;
}
