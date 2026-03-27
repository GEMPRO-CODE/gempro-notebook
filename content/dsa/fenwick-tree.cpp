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
struct Fenwick {
	vector<ll> t;
	int n;
	Fenwick(int N) : t(N), n(N) {}
	void add(int i, ll val) {
		for (i++; i <= n; i += i & -i) t[i - 1] += val;
	}
	ll sum(int r) {
		ll s = 0;
		for (; r; r -= r & -r) s += t[r - 1];
		return s;
	}
	ll sum(int l, int r) { return sum(r) - sum(l); }
};
// end template //
