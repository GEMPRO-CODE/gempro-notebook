#include <vector>
#include <algorithm>
#include <utility>
using namespace std;
using ll = long long;

// begin template //
struct Fenwick2D {
	vector<ll> x;
	vector<vector<ll>> y, t;
	int pos(vector<ll> &v, ll a) {
		return lower_bound(v.begin(), v.end(), a)-v.begin();
	}
	Fenwick2D(vector<pair<ll,ll>> pts) {
		for (auto [a,b] : pts) x.push_back(a);
		sort(x.begin(), x.end());
		x.erase(unique(x.begin(), x.end()), x.end());
		int n = x.size();
		y.resize(n+1); t.resize(n+1);
		for (auto [a,b] : pts)
			for (int i = pos(x,a)+1; i <= n; i += i&-i)
				y[i].push_back(b);
		for (int i = 1; i <= n; ++i) {
			sort(y[i].begin(), y[i].end());
			y[i].erase(unique(y[i].begin(), y[i].end()), y[i].end());
			t[i].resize(y[i].size()+1);
		}
	}
	void add(ll a, ll b, ll v) {
		for (int i = pos(x,a)+1; i < int(t.size()); i += i&-i)
			for (int j = pos(y[i],b)+1; j < int(t[i].size()); j += j&-j)
				t[i][j] += v;
	}
	ll sum(ll a, ll b) {
		ll s = 0;
		for (int i = pos(x,a); i; i -= i&-i)
			for (int j = pos(y[i],b); j; j -= j&-j)
				s += t[i][j];
		return s;
	}
	ll rect(ll a, ll b, ll c, ll d) {
		return sum(c,d)-sum(a,d)-sum(c,b)+sum(a,b);
	}
};
// end template //
