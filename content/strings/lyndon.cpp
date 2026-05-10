#include <vector>
#include <iostream>
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
vi lyndon(string s) {
	int n = sz(s);
	vi p = {0};
	for (int i = 0; i < n;) {
		int j = i + 1, k = i;
		while (j < n && s[k] <= s[j]) {
			if (s[k] < s[j]) k = i;
			else k++;
			j++;
		}
		while (i <= k) p.pb(i += j - k);
	}
	return p;
}
// end template //

// Test at: https://judge.yosupo.jp/problem/lyndon_factorization

int main() {
	cin.tie(0)->sync_with_stdio(0);
	string s;
	cin >> s;
	auto v = lyndon(s);
	for (int i: v) cout << i << ' ';
	cout << endl;
}
