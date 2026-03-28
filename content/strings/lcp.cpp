#include <string>
#include <vector>
#include <algorithm>
#include <random>

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

// begin template //
vi lcpArr(string &s, vi &sa) {
	int n = sz(s), k = 0;
	vi rnk(n), lcp(n);
	rep(i,0,n) rnk[sa[i]] = i;
	rep(i,0,n) if (rnk[i]) {
		int j = sa[rnk[i] - 1];
		while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
		lcp[rnk[i]] = k;
		if (k) k--;
	}
	return lcp;
}
// end template //
