#include <iostream>
#include <random>

using namespace std;

// begin template //
static mt19937 rng;

template<class T>
struct RBST {
	struct Node {
		T v;
		int cnt = 1;
		Node *ch[2] = {0, 0};
		~Node() {
			for (int i: {0, 1}) if (ch[i]) delete ch[i];
		}
	};
	static Node * update(Node *n) {
		if (!n) return 0;
		n->cnt = 1;
		for (int i: {0, 1}) n->cnt += n->ch[i] ? n->ch[i]->cnt : 0;
		return n;
	}
	static Node *merge(Node *l, Node *r) {
		if (!l || !r) return l ? l : r;
		if (rng() % (l->cnt + r->cnt) < l->cnt) {
			l->ch[1] = merge(l->ch[1], r);
			return update(l);
		} else {
			r->ch[0] = merge(l, r->ch[0]);
			return update(r);
		}
	}
	static void split(Node *rt, auto cond, Node *&l, Node *&r) {
		if (!rt) return;
		if (cond(rt->v)) {
			r = rt, rt = r->ch[0], r->ch[0] = 0;
			split(rt, cond, l, r->ch[0]);
			update(r);
		} else {
			l = rt, rt = l->ch[1], l->ch[1] = 0;
			split(rt, cond, l->ch[1], r);
			update(l);
		}
	}
	static void splitAt(Node *rt, int i, Node *&l, Node *&r) {
		if (!rt) return;
		int k = rt->ch[0] ? rt->ch[0]->cnt : 0;
		if (i <= k) {
			r = rt, rt = r->ch[0], r->ch[0] = 0;
			splitAt(rt, i, l, r->ch[0]);
			update(r);
			return;
		}
		i -= k + 1;
		l = rt, rt = l->ch[1], l->ch[1] = 0;
		splitAt(rt, i, l->ch[1], r);
		update(l);
	}
	static Node *front(Node * rt) {
		if (!rt || !rt->ch[0]) return rt;
		return front(rt->ch[0]);
	}
	static Node *back(Node *rt) {
		if (!rt || !rt->ch[1]) return rt;
		return back(rt->ch[1]);
	}
	Node *rt = 0;
	RBST() : rt(0) {}
};
// end template //

using Set = RBST<int>;

signed main() {
	cin.tie(0)->sync_with_stdio(0);
	Set st;
	auto splitOn = [&](int x) {
		Set::Node *l = 0, *r = 0;
		Set::split(st.rt, [&](int y) { return x < y; }, l, r);
		return pair(l, r);
	};
	auto splitAt = [&](int i) {
		Set::Node *l = 0, *r = 0;
		Set::splitAt(st.rt, i, l, r);
		return pair(l, r);
	};
	auto contains = [&](int x) {
		auto [l, r] = splitOn(x);
		bool ret = l && Set::back(l)->v == x;
		st.rt = Set::merge(l, r);
		return ret;
	};
	auto insert = [&](int x) {
		if (contains(x)) return;
		auto [l, r] = splitOn(x);
		auto m = new Set::Node(x);
		l = Set::merge(l, m);
		st.rt = Set::merge(l, r);
	};
	auto erase = [&](int x) {
		if (!contains(x)) return;
		auto [l, r] = splitOn(x);
		st.rt = l;
		auto [ll, lr] = splitOn(x - 1);
		delete lr;
		st.rt = Set::merge(ll, r);
	};
	auto kth = [&](int k) {
		if (!st.rt || st.rt->cnt < k) return -1;
		auto [l, r] = splitAt(k);
		int value = Set::back(l)->v;
		st.rt = Set::merge(l, r);
		return value;
	};
	auto cntLess = [&](int x) {
		auto [l, r] = splitOn(x);
		int cnt = l ? l->cnt : 0;
		st.rt = Set::merge(l, r);
		return cnt;
	};
	auto lastLE = [&](int x) {
		auto [l, r] = splitOn(x);
		auto n = Set::back(l);
		st.rt = Set::merge(l, r);
		return n ? n->v : -1;
	};
	auto firstGE = [&](int x) {
		auto [l, r] = splitOn(x - 1);
		auto n = Set::front(r);
		st.rt = Set::merge(l, r);
		return n ? n->v : -1;
	};
	int n, q;
	cin >> n >> q;
	for (int i = 0; i < n; i++) {
		int a;
		cin >> a;
		insert(a);
	}
	for (int i = 0; i < q; i++) {
		int t, x;
		cin >> t >> x;
		if (t == 0) {
			insert(x);
		} else if (t == 1) {
			erase(x);
		} else if (t == 2) {
			cout << kth(x) << endl;
		} else if (t == 3) {
			cout << cntLess(x) << endl;
		} else if (t == 4) {
			cout << lastLE(x) << endl;
		} else if (t == 5) {
			cout << firstGE(x) << endl;
		}
	}
	delete st.rt;
	return 0;
}
