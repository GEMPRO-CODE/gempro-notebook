#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

using namespace std;

// begin template //

vector<int> sufArr(string &s) { // Could also be vector<T> &s
	int n = s.size();
	vector<int> c(n), d(n), e(n), sb(n), sa(n), cnt(n + 1);
	iota(sa.begin(), sa.end(), 0);
	sort(sa.begin(), sa.end(), [&](int i, int j) { return s[i] < s[j]; });
	c[sa[0]] = 1;
	for (int i = 1; i < n; i++)
		c[sa[i]] = c[sa[i - 1]] + (s[sa[i]] != s[sa[i - 1]]);
	for (int k = 1; c[sa[n - 1]] != n; k <<= 1) {
		for (int i = 0; i < n; i++) d[i] = i + k < n ? c[i + k] : 0;
		auto srt = [&](auto &C) {
			fill(cnt.begin(), cnt.end(), 0);
			for (int i = 0; i < n; i++) cnt[C[i] + 1]++;
			for (int i = 1; i <= n; i++) cnt[i] += cnt[i - 1];
			for (int i: sa) sb[cnt[C[i]]++] = i;
			swap(sa, sb);
		};
		srt(d); srt(c);
		int lc = 0, ld = 0, le = 0;
		for (int i: sa) {
			if (c[i] != lc || d[i] != ld) e[i] = le + 1;
			else e[i] = le;
			lc = c[i], ld = d[i], le = e[i];
		}
		swap(c, e);
	}
	return sa;
}

// end template //
