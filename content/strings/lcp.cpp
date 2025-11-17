#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// begin template //

vector<int> lcpArr(string &s, vector<int> &sa) {
	int n = s.size(), k = 0;
	vector<int> r(n), lcp(n);
	for (int i = 0; i < n; i++) r[sa[i]] = i;
	for (int i = 0; i < n; i++) {
		if (r[i] == n - 1) {
			k = 0;
			continue;
		}
		int j = sa[r[i] + 1];
		while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
		lcp[r[i]] = k;
		if (k) k--;
	}
	return lcp;
}

// end template //
