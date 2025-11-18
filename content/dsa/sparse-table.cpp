#include <vector>

using namespace std;

// begin template //
template <class S, S (*op)(S, S)> struct SparseTable {
	vector<vector<S>> t;
	vector<int> lg;
	void build(vector<S> &v) {
		int n = ssize(v);
		lg.assign(n + 1, 0);
		for (int i = 2; i <= n; i++) lg[i] = lg[i >> 1] + 1;
		t.assign(lg[n] + 1, vector<S>(n));
		t[0] = v;
		for (int k = 1; k <= lg[n]; k++)
			for (int i = 0; i + (1 << k) <= n; i++)
				t[k][i] = op(t[k - 1][i], t[k - 1][i + (1 << (k - 1))]);
	}
	int query(int l, int r) {
		int k = lg[r - l];
		return op(t[k][l], t[k][r - (1 << k)]);
	}
};
// end template //
