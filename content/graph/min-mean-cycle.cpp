// Karp's minimum cycle mean formula; all vertices are initial sources.
// https://www.cs.colostate.edu/~rmm/minCycleMean.pdf
#include <algorithm>
#include <optional>
#include <tuple>
#include <utility>
#include <vector>
using namespace std;
using ll = long long;
using i128 = __int128_t;

// begin template //
optional<pair<i128,int>> minMeanCycle(
		int n, const vector<tuple<int,int,ll>>& edges) {
	const i128 inf = i128(1)<<120;
	vector<vector<i128>> d(n+1,vector<i128>(n,inf));
	fill(d[0].begin(),d[0].end(),0);
	for (int k = 1; k <= n; ++k)
		for (auto [u,v,w] : edges) if (d[k-1][u] != inf)
			d[k][v] = min(d[k][v],d[k-1][u]+w);
	using Fraction = pair<i128,int>;
	auto less = [](Fraction a, Fraction b) {
		return a.first*b.second < b.first*a.second;
	};
	optional<Fraction> ans;
	for (int v = 0; v < n; ++v) if (d[n][v] != inf) {
		Fraction worst = {d[n][v]-d[0][v],n};
		for (int k = 1; k < n; ++k) if (d[k][v] != inf) {
			Fraction cur = {d[n][v]-d[k][v],n-k};
			if (less(worst,cur)) worst = cur;
		}
		if (!ans || less(worst,*ans)) ans = worst;
	}
	return ans;
}
// end template //
