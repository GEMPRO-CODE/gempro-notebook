#include <vector>
#include <random>
#include <iostream>
#include <array>

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
struct SAM {
	struct N {
		int len = 0, link = -1;
		array<int, 26> to;
		ll cnt = 0; // endpos count after calcCnt()
		N() { to.fill(-1); }
	};
	vector<N> t{{}};
	int last = 0;
	void extend(char c) {
		int x = c - 'a', cur = sz(t), p = last;
		t.eb();
		t[cur].len = t[last].len + 1;
		t[cur].cnt = 1;
		for (; p != -1 && t[p].to[x] == -1; p = t[p].link) t[p].to[x] = cur;
		if (p == -1) t[cur].link = 0;
		else {
			int q = t[p].to[x];
			if (t[p].len + 1 == t[q].len) t[cur].link = q;
			else {
				int cl = sz(t);
				t.pb(t[q]);
				t[cl].len = t[p].len + 1;
				t[cl].cnt = 0;
				for (; p != -1 && t[p].to[x] == q; p = t[p].link) t[p].to[x] = cl;
				t[q].link = t[cur].link = cl;
			}
		}
		last = cur;
	}
	vi ord() {
		int mx = 0;
		for (auto &u : t) mx = max(mx, u.len);
		vi cnt(mx + 1), ord(sz(t));
		for (auto &u : t) cnt[u.len]++;
		rep(i,1,mx+1) cnt[i] += cnt[i - 1];
		for (int i = sz(t) - 1; i >= 0; i--) ord[--cnt[t[i].len]] = i;
		return ord;
	}
};
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
}
