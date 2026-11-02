#include <vector>
#include <cstdint>
using namespace std;

// begin template //
template<int B = 30> struct XorTrie {
	using T = uint64_t;
	struct Node { int ch[2] = {-1,-1}, cnt = 0; };
	vector<Node> t{Node{}};
	int size() const { return t[0].cnt; }
	int count(T x) const {
		int p = 0;
		for (int i = B-1; i >= 0 && p >= 0; --i) p = t[p].ch[x>>i&1];
		return p < 0 ? 0 : t[p].cnt;
	}
	void add(T x, int d = 1) {
		int p = 0; t[p].cnt += d;
		for (int i = B-1; i >= 0; --i) {
			int b = x>>i&1;
			if (t[p].ch[b] < 0) {
				t[p].ch[b] = t.size(); t.emplace_back();
			}
			p = t[p].ch[b]; t[p].cnt += d;
		}
	}
	T kth(int k, T x = 0) const {
		int p = 0;
		T ans = 0;
		for (int i = B-1; i >= 0; --i) {
			int b = x>>i&1, q = t[p].ch[b];
			int c = q < 0 ? 0 : t[q].cnt;
			if (k >= c) k -= c, b ^= 1, ans |= T(1)<<i;
			p = t[p].ch[b];
		}
		return ans;
	}
};
// end template //
