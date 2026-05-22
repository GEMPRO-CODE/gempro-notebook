#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <cassert>

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
ll perm2int(vi p) {
	vi a(sz(p)); iota(all(a), 0); ll r = 0;
	for (int x: p) {
		int i = find(all(a), x) - a.begin();
		r = r * sz(a) + i;
		a.erase(a.begin() + i);
	}
	return r;
}

vi int2perm(int n, ll k) {
	vi a(n), p; iota(all(a), 0); ll f = 1;
	rep(i,1,n) f *= i;
	rep(i,0,n) {
		int j = k / f; k %= f;
		p.pb(a[j]); a.erase(a.begin() + j);
		if (n - i - 1) f /= n - i - 1;
	}
	return p;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int t = 100;
	int n = 10;
	vector<int> perm(n);
	iota(begin(perm), end(perm), 0);
	while (t--) {
		shuffle(begin(perm), end(perm), rng);
		assert(int2perm(n, perm2int(perm)) == perm);
	}
	for (int i = 0; i < 720; i++) {
		assert(perm2int(int2perm(6, i)) == i);
	}
}
