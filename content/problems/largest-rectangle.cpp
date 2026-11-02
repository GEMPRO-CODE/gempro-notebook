#include <array>
#include <vector>
using namespace std;
using ll = long long;

// begin template //
array<int,4> largestRectangle(const vector<vector<int>> &a) {
	int n = a.size(), m = n ? a[0].size() : 0;
	vector<int> h(m+1), st;
	array<int,4> ans{};
	ll best = 0;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) h[j] = a[i][j] ? h[j]+1 : 0;
		st.clear();
		for (int j = 0; j <= m; ++j) {
			while (!st.empty() && h[st.back()] >= h[j]) {
				int height = h[st.back()]; st.pop_back();
				int l = st.empty() ? 0 : st.back()+1;
				ll area = ll(height)*(j-l);
				if (area > best) best = area, ans = {i+1-height,l,i+1,j};
			}
			st.push_back(j);
		}
	}
	return ans;
}
// end template //
