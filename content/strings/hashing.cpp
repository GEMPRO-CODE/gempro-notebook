#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <compare>
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
struct H {
	static constexpr ll M = (1LL<<61)-1;
	inline static const ll base = 256+mt19937_64(random_device{}())()%(M-257);
	ll h = 0, pw = 1;
	int cnt = 0;
	H() = default;
	H(ll x, int n=1, ll p=base): h((x%M+M)%M), pw(p), cnt(n) {}
	static ll mul(ll a, ll b) { return (__int128)a*b%M; }
	H concat(H b) const {
		return {mul(h,b.pw)+b.h, cnt+b.cnt, mul(pw,b.pw)};
	}
	bool operator==(const H&) const = default;
};
// end template //
