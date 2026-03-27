#include <vector>
#include <random>
#include <string>

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
vi kmp(string s) {
	int n = sz(s), len = 0;
	vi pre(n);
	rep (i, 1, n) {
		while (len > 0 && s[i] != s[len]) len = pre[len - 1];
		if (s[i] == s[len]) len++;
		pre[i] = len;
	}
	return pre;
}
// end template //
