#include <vector>
#include <tuple>
#include <deque>
#include <climits>
using namespace std;
using ll = long long;

// begin template //
vector<ll> boundedKnapsack(int W, vector<tuple<int,ll,int>> items) {
	vector<ll> dp(W+1, LLONG_MIN);
	dp[0] = 0;
	for (auto [w,v,c] : items) {
		if (w > W || !c) continue;
		auto old = dp;
		for (int r = 0; r < w; ++r) {
			deque<pair<int,ll>> q;
			for (int s = r, j = 0; s <= W; s += w, ++j) {
				while (!q.empty() && q.front().first < j-c) q.pop_front();
				if (old[s] != LLONG_MIN) {
					ll x = old[s]-j*v;
					while (!q.empty() && q.back().second <= x) q.pop_back();
					q.emplace_back(j, x);
				}
				dp[s] = q.empty() ? LLONG_MIN : q.front().second+j*v;
			}
		}
	}
	return dp;
}
// end template //
