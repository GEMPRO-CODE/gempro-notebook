#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <utility>
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
#define rep(i,a,b) for (int i = (a); i < (b); ++i)

mt19937 rng(random_device{}());

// begin template //
pair<vi, vi> manacher(string& s) {
	int n = sz(s);
	vi d1(n), d2(n);
	for (int i = 0, l = 0, r = -1; i < n; i++) {
		int k = i > r ? 1 : min(d1[l + r - i], r - i + 1);
		while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) k++;
		d1[i] = k--;
		if (i + k > r) l = i - k, r = i + k;
	}
	for (int i = 0, l = 0, r = -1; i < n; i++) {
		int k = i > r ? 0 : min(d2[l + r - i + 1], r - i + 1);
		while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) k++;
		d2[i] = k--;
		if (i + k > r) l = i - k - 1, r = i + k;
	}
	return {d1, d2}; // {odd, even}
}
// end template //

// Test at: https://judge.yosupo.jp/problem/enumerate_palindromes

int main() {
	cin.tie(0)->sync_with_stdio(0);
	string s;
	cin >> s;
	int n = sz(s);
	auto [a, b] = manacher(s);
	cout << a[0] << ' ';
	rep(i, 1, n) {
		cout << 2 * b[i] << ' ' << 2 * a[i] - 1 << ' ';
	}
	cout << endl;
}
