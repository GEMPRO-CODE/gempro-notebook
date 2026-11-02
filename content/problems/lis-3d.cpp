#include <algorithm>
#include <array>
#include <numeric>
#include <vector>
using namespace std;
using ll = long long;

// begin template //
vector<int> lis3D(const vector<array<ll,3>> &p, vector<ll> w = {}) {
	int n = p.size();
	if (!n) return {};
	if (w.empty()) w.assign(n,1);
	vector<int> ord(n), z(n), par(n,-1);
	vector<ll> zs, dp = w;
	for (auto a : p) zs.push_back(a[2]);
	sort(zs.begin(),zs.end());
	zs.erase(unique(zs.begin(),zs.end()),zs.end());
	for (int i = 0; i < n; ++i)
		z[i] = lower_bound(zs.begin(),zs.end(),p[i][2])-zs.begin()+1;
	iota(ord.begin(),ord.end(),0);
	sort(ord.begin(),ord.end(),[&](int i,int j) {
		return p[i][0] != p[j][0] ? p[i][0] < p[j][0] : p[i][1] > p[j][1];
	});
	vector<pair<ll,int>> bit(zs.size()+1,{0,-1});
	auto cdq = [&](auto &&self,int l,int r) -> void {
		if (r-l < 2) return;
		int m = (l+r)/2;
		self(self,l,m);
		vector<int> ids(r-l); iota(ids.begin(),ids.end(),l);
		sort(ids.begin(),ids.end(),[&](int i,int j) {
			ll a = p[ord[i]][1], b = p[ord[j]][1];
			return a != b ? a < b : i > j;
		});
		for (int i : ids) {
			int u = ord[i];
			if (i < m) {
				for (int k = z[u]; k < (int)bit.size(); k += k&-k)
					bit[k] = max(bit[k],pair{dp[u],u});
			} else for (int k = z[u]-1; k; k -= k&-k)
				if (bit[k].first+w[u] > dp[u])
					dp[u] = bit[k].first+w[u], par[u] = bit[k].second;
		}
		for (int i = l; i < m; ++i)
			for (int k = z[ord[i]]; k < (int)bit.size(); k += k&-k) bit[k] = {0,-1};
		self(self,m,r);
	};
	cdq(cdq,0,n);
	vector<int> ans;
	for (int u = max_element(dp.begin(),dp.end())-dp.begin(); u >= 0; u = par[u])
		ans.push_back(u);
	reverse(ans.begin(),ans.end());
	return ans;
}
// end template //
