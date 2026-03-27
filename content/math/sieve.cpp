#include <vector>
#include <random>

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
struct Sieve {
	vi primes, spf;
	Sieve(int n): spf(n) {
		rep (i, 2, n) {
			if (!spf[i]) {
				spf[i] = i;
				primes.push_back(i);
			}
			for (int j: primes) {
				if (j * i >= n) break;
				spf[i * j] = j;
				if (j == spf[i]) break;
			}
		}
	}
};
// end template //
