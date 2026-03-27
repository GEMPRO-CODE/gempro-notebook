#include <random>
#include <iostream>

using namespace std;

mt19937 rng(random_device{}());

using i32 = int;
using i64 = long long;

// begin template //
struct TS {
    i64 v;
	i32 pr, sz;
    TS *l, *r;
    TS(i64 V) : v(V), pr((rand()<<16)^rand()), sz(1), l(0), r(0) {}
};

int sz(TS* t) { return t ? t->sz : 0; }
void pull(TS* t) { if (t) t->sz = 1 + sz(t->l) + sz(t->r); }

void split(TS* t, int key, TS*& l, TS*& r) {
    if (!t) l = r = 0;
    else if (t->v <= key) split(t->r, key, t->r, r), l = t;
    else split(t->l, key, l, t->l), r = t;
    pull(t);
}

TS* merge(TS* l, TS* r) {
    if (!l || !r) return l ? l : r;
    if (l->pr > r->pr) {
        l->r = merge(l->r, r);
        pull(l);
        return l;
    } else {
        r->l = merge(l, r->l);
        pull(r);
        return r;
    }
}

bool find(TS* t, int v) {
    while (t) {
        if (t->v == v) return 1;
        t = v < t->v ? t->l : t->r;
    }
    return 0;
}

void insert(TS*& t, TS* it) {
    if (!t) t = it;
    else if (it->pr > t->pr) split(t, it->v, it->l, it->r), t = it;
    else insert(it->v < t->v ? t->l : t->r, it);
    pull(t);
}

void erase(TS*& t, int v) {
    if (!t) return;
    if (t->v == v) t = merge(t->l, t->r);
    else erase(v < t->v ? t->l : t->r, v);
    pull(t);
}

int kth(TS* t, int k) { // 1-indexed, assumes 1 <= k <= sz(t)
    if (sz(t->l) + 1 == k) return t->v;
    if (k <= sz(t->l)) return kth(t->l, k);
    return kth(t->r, k - sz(t->l) - 1);
}

int cntLE(TS* t, i64 v) {
    if (!t) return 0;
    if (t->v <= v) return sz(t->l) + 1 + cntLE(t->r, v);
    return cntLE(t->l, v);
}

TS* pred(TS* t, i64 v) { // max key <= x
    TS* ans = 0;
    while (t) {
        if (t->v <= v) ans = t, t = t->r;
        else t = t->l;
    }
    return ans;
}

TS* succ(TS* t, i64 v) { // min key >= x
    TS* ans = 0;
    while (t) {
        if (t->v >= v) ans = t, t = t->l;
        else t = t->r;
    }
    return ans;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n, q;
	cin >> n >> q;
	TS *t = 0;
	for (int i = 0; i < n; i++) {
		int a;
		cin >> a;
		insert(t, new TS(a));
	}
	while (q--) {
		int k, x;
		cin >> k >> x;
		if (k == 0) {
			insert(t, new TS(x));
		} else if (k == 1) {
			erase(t, x);
		} else if (k == 2) {
			if (sz(t) < x) cout << -1 << endl;
			else cout << kth(t, x) << endl;
		} else if (k == 3) {
			cout << cntLE(t, x) << endl;
		} else if (k == 4) {
			if (auto n = pred(t, x)) {
				cout << n->v << endl;
			} else {
				cout << -1 << endl;
			}
		} else if (k == 5) {
			if (auto n = succ(t, x)) {
				cout << n->v << endl;
			} else {
				cout << -1 << endl;
			}
		}
	}
}
