#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

using namespace std;

using ll = long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;

#define pb push_back
#define eb emplace_back
#define fi first
#define se second
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)

mt19937 rng(random_device{}());

// begin template //
struct Node;
using TN = Node*;

struct Node {
	int y = rng(), cnt, v;
	ll sum;
	bool rev = 0;
	TN l = 0, r = 0;
	Node(int v) : cnt(1), v(v), sum(v) {}
};

int cnt(TN t) { return t ? t->cnt : 0; }
ll sum(TN t) { return t ? t->sum : 0; }

void apply_rev(TN t) {
	if (!t) return;
	t->rev ^= 1;
	swap(t->l, t->r);
}
void push(TN t) {
	if (!t || !t->rev) return;
	apply_rev(t->l);
	apply_rev(t->r);
	t->rev = 0;
}
void pull(TN t) {
	t->cnt = 1 + cnt(t->l) + cnt(t->r);
	t->sum = t->v + sum(t->l) + sum(t->r);
}

void split(TN t, int k, TN& a, TN& b) {
	if (!t) return a = b = 0, void();
	push(t);
	if (cnt(t->l) >= k) split(t->l, k, a, t->l), b = t;
	else split(t->r, k - cnt(t->l) - 1, t->r, b), a = t;
	pull(t);
}
TN merge(TN a, TN b) {
	if (!a || !b) return a ? a : b;
	if (a->y > b->y) return push(a), a->r = merge(a->r, b), pull(a), a;
	return push(b), b->l = merge(a, b->l), pull(b), b;
}

void ins(TN& t, int p, int v) {
	TN a, b;
	split(t, p, a, b);
	t = merge(merge(a, new Node(v)), b);
}
void era(TN& t, int p) {
	TN a, b, c;
	split(t, p, a, b);
	split(b, 1, b, c);
	delete b;
	t = merge(a, c);
}
TN kth(TN t, int p) {
	while (t) {
		push(t);
		if (cnt(t->l) == p) return t;
		if (cnt(t->l) > p) t = t->l;
		else p -= cnt(t->l) + 1, t = t->r;
	}
	return 0;
}

void sep(TN t, int l, int r, TN& a, TN& b, TN& c) {
	split(t, r, b, c);
	split(b, l, a, b);
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n, q;
	cin >> n >> q;
	vector<int> a(n);
	for (int i = 0; i < n; i++) cin >> a[i];
	TN rt = 0;
	for (int i = 0; i < n; i++) ins(rt, i, a[i]);
	for (int i = 0; i < q; i++) {
		int t, l, r;
		cin >> t >> l >> r;
		if (t == 0) {
			TN a, b, c;
			sep(rt, l, r, a, b, c);
			apply_rev(b);
			rt = merge(merge(a, b), c);
		} else if (t == 1) {
			TN a, b, c;
			sep(rt, l,  r, a, b, c);
			cout << sum(b) << endl;
			rt = merge(merge(a, b), c);
		} else assert(false);
	}
}
