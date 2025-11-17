#include <vector>

using namespace std;

// begin template //
struct Fenwick {
	vector<int> t;
	int n;
	Fenwick(int N) : t(N), n(N) {}
	void add(int i, int val) {
		for (i++; i <= n; i += i & -i) t[i - 1] += val;
	}
	int sum(int r) {
		int s = 0;
		for (; r; r -= r & -r) s += t[r - 1];
		return s;
	}
	int sum(int l, int r) {
		return sum(r) - sum(l);
	}
};
// end template //
