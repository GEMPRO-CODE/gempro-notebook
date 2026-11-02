#include <vector>
#include <array>
#include <tuple>
#include <algorithm>
using namespace std;
using ll = long long;
using i128 = __int128_t;

// begin template //
i128 rectUnion(vector<array<ll,4>> rects) {
	vector<tuple<ll,int,ll,ll>> ev;
	vector<ll> y;
	for (auto [a,b,c,d] : rects) if (a < c && b < d) {
		ev.emplace_back(a, 1, b, d); ev.emplace_back(c, -1, b, d);
		y.push_back(b); y.push_back(d);
	}
	if (ev.empty()) return 0;
	sort(ev.begin(), ev.end()); sort(y.begin(), y.end());
	y.erase(unique(y.begin(), y.end()), y.end());
	int n = y.size()-1;
	vector<int> cnt(4*n);
	vector<ll> len(4*n);
	auto upd = [&](auto &&self, int p, int l, int r, int a, int b, int d) -> void {
		if (b <= l || r <= a) return;
		if (a <= l && r <= b) cnt[p] += d;
		else {
			int m = (l+r)/2;
			self(self, 2*p, l, m, a, b, d);
			self(self, 2*p+1, m, r, a, b, d);
		}
		len[p] = cnt[p] ? y[r]-y[l] : (r-l == 1 ? 0 : len[2*p]+len[2*p+1]);
	};
	i128 area = 0;
	ll last = get<0>(ev[0]);
	for (auto [x,d,a,b] : ev) {
		area += i128(x-last)*len[1]; last = x;
		int l = lower_bound(y.begin(), y.end(), a)-y.begin();
		int r = lower_bound(y.begin(), y.end(), b)-y.begin();
		upd(upd, 1, 0, n, l, r, d);
	}
	return area;
}
// end template //
