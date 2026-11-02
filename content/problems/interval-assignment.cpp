#include <map>
#include <iterator>
using namespace std;
using ll = long long;

// begin template //
struct Intervals {
	ll n;
	map<ll,ll> m;
	Intervals(ll n, ll x = 0): n(n), m{{0,x},{n,0}} {}
	auto split(ll p) {
		auto it = m.lower_bound(p);
		if (it->first == p) return it;
		return m.emplace(p, prev(it)->second).first;
	}
	void assign(ll l, ll r, ll x) {
		if (l == r) return;
		auto b = split(r), a = split(l);
		m.erase(a, b);
		a = m.emplace(l, x).first;
		if (a != m.begin() && prev(a)->second == x) {
			a = prev(a); m.erase(next(a));
		}
		b = next(a);
		if (b->first < n && b->second == x) m.erase(b);
	}
	ll get(ll p) { return prev(m.upper_bound(p))->second; }
};
// end template //
