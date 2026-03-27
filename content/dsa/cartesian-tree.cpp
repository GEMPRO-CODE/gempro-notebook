#include <bits/stdc++.h>

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
vi cartTree(int n, auto le) {
	int last;
	vi st, par(n);
	rep (i, 0, n) { 
		par[i] = i, last = -1;
		while (sz(st) && le(par[i], st.back())) {
			if (last != -1) par[last] = st.back();
			last = st.back();
			st.pop_back();
		}
		if (last != -1) par[last] = i;
		st.push_back(i);
	}
	while (sz(st) > 1) {
		last = st.back();
		st.pop_back();
		par[last] = st.back();
	}
	return par;
}
// end template //
