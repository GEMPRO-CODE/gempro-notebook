#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numeric>
#include <random>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
using ll = long long;
using ld = long double;
using vi = vector<int>;
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)

// begin template //
struct RollbackChmax {
	using Value = tuple<int,int,ll>;
	struct Node { ll mn, tag; };
	vector<Node> t;
	vector<pair<int,Node>> log;
	vector<int> marks;
	int n;
	RollbackChmax(int n, ll initial=0)
		: t(4*n,{initial,LLONG_MIN}), n(n) {}
	void update(int l, int r, ll x, int i, int a, int b) {
		if (r <= a || b <= l || t[i].mn >= x) return;
		log.push_back({i,t[i]});
		if (l <= a && b <= r) {
			t[i].mn = max(t[i].mn,x);
			t[i].tag = max(t[i].tag,x); return;
		}
		int m = (a+b)/2;
		update(l,r,x,2*i,a,m); update(l,r,x,2*i+1,m,b);
		t[i].mn = max(t[i].tag,min(t[2*i].mn,t[2*i+1].mn));
	}
	void insert(Value v) {
		marks.push_back(sz(log));
		auto [l,r,x] = v; update(l,r,x,1,0,n);
	}
	void rollback() {
		while (sz(log) > marks.back()) {
			auto [i,node] = log.back(); log.pop_back(); t[i] = node;
		}
		marks.pop_back();
	}
	ll query(int l, int r, int i, int a, int b) const {
		if (r <= a || b <= l) return LLONG_MAX;
		if (l <= a && b <= r) return t[i].mn;
		int m = (a+b)/2;
		return max(t[i].tag,min(query(l,r,2*i,a,m),query(l,r,2*i+1,m,b)));
	}
	ll query(int l, int r) const { return query(l,r,1,0,n); }
};
// end template //
