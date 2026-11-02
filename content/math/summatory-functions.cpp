#include <array>
#include <vector>
#include <unordered_map>
using namespace std;
using ll = long long;
using i128 = __int128_t;

// begin template //
struct Summatory {
	vector<ll> phi, mu;
	unordered_map<ll,array<i128,2>> memo;
	Summatory(int b) : phi(b+1), mu(b+1) {
		vector<int> primes, spf(b+1);
		phi[1] = mu[1] = 1;
		for (int i = 2; i <= b; ++i) {
			if (!spf[i]) spf[i] = i, primes.push_back(i),
				phi[i] = i-1, mu[i] = -1;
			for (int p : primes) {
				if (p > b/i) break;
				spf[i*p] = p;
				phi[i*p] = phi[i]*(p-(i%p != 0));
				if (i%p == 0) break;
				mu[i*p] = -mu[i];
			}
		}
		for (int i = 1; i <= b; ++i)
			phi[i] += phi[i-1], mu[i] += mu[i-1];
	}
	array<i128,2> sum(ll n) {
		if (n < (ll)phi.size()) return {phi[n],mu[n]};
		if (memo.count(n)) return memo[n];
		array<i128,2> ans = {i128(n)*(i128(n)+1)/2,1};
		for (ll l = 2, r; l <= n; l = r+1) {
			r = n/(n/l); auto s = sum(n/l);
			for (int j = 0; j < 2; ++j) ans[j] -= (r-l+1)*s[j];
			if (r == n) break;
		}
		return memo[n] = ans;
	}
};
// end template //
