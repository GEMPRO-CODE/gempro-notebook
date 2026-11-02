#include <algorithm>
#include <array>
#include <functional>
#include <optional>
#include <queue>
#include <vector>
using namespace std;
using ll = long long;

// begin template //
optional<vector<ll>> distinctIntervals(vector<pair<ll,ll>> ranges) {
	int n = ranges.size(), j = 0;
	vector<array<ll,3>> a;
	for (int i = 0; i < n; ++i)
		a.push_back({ranges[i].first,ranges[i].second,i});
	sort(a.begin(),a.end());
	priority_queue<pair<ll,int>,vector<pair<ll,int>>,greater<>> q;
	vector<ll> ans(n);
	__int128 x = 0;
	for (int k = 0; k < n; ++k) {
		if (q.empty()) x = a[j][0];
		while (j < n && a[j][0] <= x) {
			q.emplace(a[j][1],a[j][2]); ++j;
		}
		auto [r,id] = q.top(); q.pop();
		if (x > r) return nullopt;
		ans[id] = x++;
	}
	return ans;
}
// end template //
