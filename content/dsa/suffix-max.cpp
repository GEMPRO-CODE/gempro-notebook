#include <iostream>
#include <random>
#include <vector>
#include <map>

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
template<class K, class V>
struct SuffixBest {
	map<K, V> m;
	V e;
	SuffixBest(V e = V{}): e(e) {}
	void add(K x, V y) {
		auto it = m.lower_bound(x);
		if (it != end(m) && it->se >= y) return;
		it = m.insert_or_assign(it, x, y);
		while (it != begin(m)) {
			auto p = prev(it);
			if (y < p->se) break;
			m.erase(p);
		}
	}
	V query(K x) {
		auto it = m.lower_bound(x);
		return it == end(m) ? e : it->se;
	}
};
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
