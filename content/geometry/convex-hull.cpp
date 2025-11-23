#include <vector>
#include <complex>
#include <numeric>
#include <algorithm>
#include <iostream>

using namespace std;

using i64 = long long;

using D = i64;
using VI = vector<int>;
using point = complex<D>;

D dot(point a, point b) {
	return real(conj(a) * b);
}
D cross(point a, point b) {
	return imag(conj(a) * b);
}

// begin template // 
vector<int> convHull(vector<point> &pt) {
	int n = pt.size(), m;
	VI h, ord(n);
	auto add = [&]() {
		VI st;
		for (int i: ord) {
			while ((m = st.size()) > 1) {
				point a = pt[st[m - 1]], b = pt[st[m - 2]], c = pt[i];
				if (cross(b - a, c - a) < 0) break; // > for clockwise, <= to include non-vertices
				st.pop_back();
			}
			st.push_back(i);
		}
		st.pop_back();
		h.insert(h.end(), st.begin(), st.end());
	};
	iota(ord.begin(), ord.end(), 0);
	auto top = [](auto a) { return make_pair(real(a), imag(a)); };
	sort(ord.begin(), ord.end(), [&](int i, int j) { return top(pt[i]) > top(pt[j]); });
	add();
	reverse(ord.begin(), ord.end());
	add();
	return h;
}
// end template //

// Test at https://judge.yosupo.jp/problem/static_convex_hull

istream &operator>>(istream &in, point &pt) {
	i64 x, y;
	cin >> x >> y;
	pt = point(x, y);
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
		vector<point> pt(n);
		for (int i = 0; i < n; i++) cin >> pt[i];
		auto ch = convHull(pt);
		if (ch.size() == 0) {
			cout << 1 << endl;
			cout << real(pt[0]) << ' ' << imag(pt[0]) << endl;
			continue;
		}
		vector<point> ans;
		point last(1e12, 1e12);
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
