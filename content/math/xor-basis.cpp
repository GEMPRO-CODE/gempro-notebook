#include <vector>
#include <utility>
#include <bitset>
#include <random>
#include <iostream>

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
template<int B = 63>
struct XorBasis {
	using T = bitset<B>;
	struct Row { int i; T x,w; };
	vector<Row> b;
	T reduce(T &x, bool hi = 0) const {
		T w;
		for (auto &r : b) if (x[r.i] != hi) x ^= r.x, w ^= r.w;
		return w;
	}
	bool add(T x) {
		auto w = reduce(x);
		for (int i = B-1; i >= 0; --i) if (x[i]) {
			w[sz(b)] = 1;
			int j = 0;
			while (j < sz(b) && b[j].i > i) ++j;
			b.insert(b.begin()+j,{i,x,w}); return 0;
		}
		return 1;
	}
	T query(T x = {}, bool hi = 0) const { reduce(x,hi); return x; }
};
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
