#include <vector>
#include <string>

using namespace std;

using VI = vector<int>;

// begin template //
VI kmp(string s) {
	int n = s.size(), len = 0;
	VI pre(n);
	for (int i = 1; i < n; i++) {
		while (len > 0 && s[i] != s[len]) len = pre[len - 1];
		if (s[i] == s[len]) len++;
		pre[i] = len;
	}
	return pre;
}
// end template //
