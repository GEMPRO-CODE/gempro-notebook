#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
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
vi sufArr(string &s) { // Could also be vector<T> &s
	int n = sz(s);
	vi c(n), d(n), e(n), sb(n), sa(n), cnt(n + 1);
	iota(all(sa), 0);
	sort(all(sa), [&](int i, int j) { return s[i] < s[j]; });
	c[sa[0]] = 1;
	for (int i = 1; i < n; i++)
		c[sa[i]] = c[sa[i - 1]] + (s[sa[i]] != s[sa[i - 1]]);
	for (int k = 1; c[sa[n - 1]] != n; k <<= 1) {
		rep (i, 0, n) d[i] = i + k < n ? c[i + k] : 0;
		auto srt = [&](auto &C) {
			fill(all(cnt), 0);
			rep (i, 0, n) cnt[C[i] + 1]++;
			rep (i, 0, n) cnt[i + 1] += cnt[i];
			for (int i: sa) sb[cnt[C[i]]++] = i;
			swap(sa, sb);
		};
		srt(d); srt(c);
		e[sa[0]] = 1;
		rep(i,1,n) {
			int a = sa[i-1], b = sa[i];
			e[b] = e[a] + (c[a] != c[b] || d[a] != d[b]);
		}
		swap(c, e);
	}
	return sa;
}
// end template //
