#include <vector>

using namespace std;

using VI = vector<int>;

using i64 = long long;

// begin template //
struct Fenwick {
	vector<i64> t;
	int n;
	Fenwick(int N) : t(N), n(N) {}
	void add(int i, i64 val) {
		for (i++; i <= n; i += i & -i) t[i - 1] += val;
	}
	i64 sum(int r) {
		i64 s = 0;
		for (; r; r -= r & -r) s += t[r - 1];
		return s;
	}
	i64 sum(int l, int r) {
		return sum(r) - sum(l);
	}
};
// end template //
