#include <vector>

using namespace std;

// begin template //
template<class S, S (*op)(S, S), S (*e)()>
struct Segtree {
	vector<S> t;
	int n;
	Segtree(int N): t(2 * N, e()), n(N) {}
	void set(int i, S value) {
		t[i += n] = value;
		for (i >>= 1; i; i >>= 1) t[i] = op(t[i << 1], t[i << 1 | 1]);
	}
	S query(int l, int r) {
		S al = e(), ar = e();
		for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
			if (l & 1) al = op(al, t[l++]);
			if (r & 1) ar = op(t[--r], ar);
		}
		return op(al, ar);
	}
};
// end template //
