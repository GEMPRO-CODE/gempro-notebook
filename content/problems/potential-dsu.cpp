#include <vector>
#include <optional>
#include <algorithm>
using namespace std;
using ll = long long;

// begin template //
template<bool parity = false> struct PotentialDSU {
	vector<int> p;
	vector<ll> d;
	PotentialDSU(int n): p(n, -1), d(n) {}
	ll sum(ll a, ll b) { return parity ? (a^b) : a+b; }
	ll neg(ll a) { return parity ? a : -a; }
	int root(int u) {
		if (p[u] < 0) return u;
		int v = p[u], r = root(v);
		d[u] = sum(d[u], d[v]);
		return p[u] = r;
	}
	bool merge(int u, int v, ll w) {
		int a = root(u), b = root(v);
		w = sum(sum(w, d[u]), neg(d[v]));
		if (a == b) return w == 0;
		if (p[a] > p[b]) swap(a, b), w = neg(w);
		p[a] += p[b]; p[b] = a; d[b] = w;
		return true;
	}
	optional<ll> diff(int u, int v) {
		int a = root(u), b = root(v);
		if (a != b) return nullopt;
		return sum(d[v], neg(d[u]));
	}
};
// end template //
