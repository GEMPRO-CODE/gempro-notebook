#include <algorithm>
#include <cassert>
#include <climits>
#include <vector>
using namespace std;
using ll = long long;

// begin template //
template<class S, S (*op)(S,S), S (*e)(), class U,
         bool (*mapping)(U,S&), U (*compose)(U,U), U (*id)(),
         S (*init)(ll,ll)>
struct SegtreeBeats {
	struct Node { S s; U tag; int a = -1, b = -1; };
	vector<Node> t;
	ll n;
	SegtreeBeats(ll n): n(n) { make(0,n); }
	int make(ll l, ll r) {
		t.push_back({init(l,r),id()});
		return int(t.size())-1;
	}
	void pull(int i) { t[i].s = op(t[t[i].a].s,t[t[i].b].s); }
	void allApply(int i, ll l, ll r, U u) {
		if (mapping(u,t[i].s)) {
			t[i].tag = compose(u,t[i].tag); return;
		}
		assert(r-l > 1);
		push(i,l,r);
		ll m = l+(r-l)/2;
		allApply(t[i].a,l,m,u); allApply(t[i].b,m,r,u);
		pull(i);
	}
	void push(int i, ll l, ll r) {
		ll m = l+(r-l)/2;
		if (t[i].a < 0) {
			int a = make(l,m), b = make(m,r);
			t[i].a = a; t[i].b = b;
		}
		U u = t[i].tag; t[i].tag = id();
		allApply(t[i].a,l,m,u); allApply(t[i].b,m,r,u);
	}
	void set(ll p, S x) { set(p,x,0,0,n); }
	void set(ll p, S x, int i, ll l, ll r) {
		if (r-l == 1) { t[i].s = x; t[i].tag = id(); return; }
		push(i,l,r);
		ll m = l+(r-l)/2;
		if (p < m) set(p,x,t[i].a,l,m);
		else set(p,x,t[i].b,m,r);
		pull(i);
	}
	void apply(ll l, ll r, U u) {
		if (l < r) apply(l,r,u,0,0,n);
	}
	void apply(ll l, ll r, U u, int i, ll a, ll b) {
		if (r <= a || b <= l) return;
		if (l <= a && b <= r) return allApply(i,a,b,u);
		push(i,a,b);
		ll m = a+(b-a)/2;
		apply(l,r,u,t[i].a,a,m); apply(l,r,u,t[i].b,m,b);
		pull(i);
	}
	S query(ll l, ll r) { return l < r ? query(l,r,0,0,n) : e(); }
	S query(ll l, ll r, int i, ll a, ll b) {
		if (r <= a || b <= l) return e();
		if (l <= a && b <= r) return t[i].s;
		push(i,a,b);
		ll m = a+(b-a)/2;
		return op(query(l,r,t[i].a,a,m),query(l,r,t[i].b,m,b));
	}
};
// end template //

// Example: range chmin and range sum, initially zero.
// Values must exceed LLONG_MIN; all arithmetic must fit in ll.
namespace chmin_sum {
struct S { ll sum, mx, second, count; };
S e() { return {0,LLONG_MIN,LLONG_MIN,0}; }
S leaf(ll x, ll count=1) { return {x*count,x,LLONG_MIN,count}; }
S init(ll l, ll r) { return leaf(0,r-l); }
S op(S a, S b) {
	if (!a.count) return b;
	if (!b.count) return a;
	if (a.mx < b.mx) swap(a,b);
	return {a.sum+b.sum,a.mx,
		max(a.second,a.mx == b.mx ? b.second : b.mx),
		a.count+(a.mx == b.mx ? b.count : 0)};
}
bool mapping(ll cap, S &s) {
	if (s.mx <= cap) return true;
	if (cap <= s.second) return false;
	s.sum -= (s.mx-cap)*s.count;
	s.mx = cap;
	return true;
}
ll compose(ll a, ll b) { return min(a,b); }
ll id() { return LLONG_MAX; }
using Tree = SegtreeBeats<S,op,e,ll,mapping,compose,id,init>;
}
