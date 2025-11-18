#include <vector>

using namespace std;

using VI = vector<int>;

// begin template //
struct Sieve {
	VI primes, spf;
	Sieve(int n): spf(n) {
		for (int i = 2; i < n; i++) {
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
