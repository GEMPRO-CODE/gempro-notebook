#include <iostream>
#include <random>
#include <vector>

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
#define rep(i,a,b) for (int i = (a); i < (b); i++)

mt19937 rng(random_device{}());

// begin template //
mt19937_64 rng64(random_device{}());

template<class T>
struct RBST {
	vector<int> l = {0}, r = {0};
	vector<ll> c = {0};
	vector<T> v = {T{}};
	ll cnt(int x) { return c[x]; }
	void cp(int &x) {
		l.pb(l[x]), r.pb(r[x]), c.pb(c[x]), v.pb(v[x]);
		x = sz(c) - 1;
	}
	int node(ll x) {
		l.pb(0), r.pb(0), c.pb(1), v.pb(x);
		return sz(c) - 1;
	}
	void pull(int x) {
		c[x] = cnt(l[x]) + 1 + cnt(r[x]);
	}
	int merge(int a, int b) {
		if (!a || !b) return a | b;
		bool q = rng64() % (cnt(a) + cnt(b)) < cnt(a);
		int x = q ? a : b;
		cp(x);
		if (q) r[x] = merge(r[x], b);
		else l[x] = merge(a, l[x]);
		pull(x);
		return x;
	}
	pair<int, int> split(int x, ll k) { // first k, rest
		if (!x) return {};
		cp(x);
		ll m = cnt(l[x]);
		if (k <= m) {
			auto [a, b] = split(l[x], k);
			l[x] = b;
			pull(x);
			return {a, x};
		} else {
			auto [a, b] = split(r[x], k - m - 1);
			r[x] = a;
			pull(x);
			return {x, b};
		}
	}
	int kth(int x, ll k) { // node id, 0-indexed
		while (x) {
			ll m = cnt(l[x]);
			if (k < m) x = l[x];
			else if (k == m) return x;
			else k -= m + 1, x = r[x];
		}
		return 0;
	}
};
// end template //

// Test at: https://atcoder.jp/contests/abc417/tasks/abc417_g

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int q;
	cin >> q;
	RBST<char> bst;
	int zero = bst.node('0');
	int one = bst.node('1');
	vector<int> t = {zero, one};
	const ll limit = 1ll << 60;
	while (q--) {
		int l, r;
		ll x;
		cin >> l >> r >> x;
		int a = t[l], b = t[r];
		if (bst.cnt(a) + bst.cnt(b) < limit) {
			t.push_back(bst.merge(t[l], t[r]));
		} else if (bst.cnt(a) == limit) {
			t.push_back(a);
		} else {
			ll k = limit - bst.cnt(a);
			auto [c, rest] = bst.split(b, k);
			t.push_back(bst.merge(a, c));
		}
		cout << bst.v[bst.kth(t.back(), x - 1)] << endl;
	}
}
