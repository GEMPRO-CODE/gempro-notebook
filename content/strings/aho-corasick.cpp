#include <vector>
#include <queue>
#include <random>
#include <iostream>
#include <array>
#include <utility>
#include <algorithm>

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
struct AC {
	static constexpr int A = 26;
	struct N {
		array<int, A> to{};
		int link = 0;
		N() { to.fill(-1); }
	};
	vector<N> t{{}};
	vi ord;
	int add(string& s) {
		int v = 0;
		for (char c : s) {
			int x = c - 'a';
			if (t[v].to[x] == -1) t[v].to[x] = sz(t), t.eb();
			v = t[v].to[x];
		}
		return v;
	}
	void build() {
		queue<int> q;
		rep(c,0,A) {
			int u = t[0].to[c];
			if (u == -1) t[0].to[c] = 0;
			else q.push(u);
		}
		ord.clear();
		while (sz(q)) {
			int v = q.front(); q.pop();
			ord.pb(v);
			rep(c,0,A) {
				int& u = t[v].to[c];
				if (u == -1) u = t[t[v].link].to[c];
				else t[u].link = t[t[v].link].to[c], q.push(u);
			}
		}
	}
	int next(int v, char c) { return t[v].to[c - 'a']; }
};
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
	string s;
	cin >> s;
	AC ac;
	int n;
	cin >> n;
	vi term(n), len(n);
	rep(i, 0, n) {
		string pat;
		cin >> pat;
		len[i] = sz(pat) - 1;
		term[i] = ac.add(pat);
	}
	ac.build();
	int v = 0, i = 0;
	vi pos(sz(ac.t), sz(s) + 1);
	for (char c: s) {
		i++;
		v = ac.next(v, c);
		pos[v] = min(pos[v], i);
	}
	for (int i = sz(ac.ord) - 1; i >= 0; i--) {
		int v = ac.ord[i];
		int link = ac.t[v].link;
		pos[link] = min(pos[link], pos[v]);
	}
	rep(i, 0, n) {
		cout << (pos[term[i]] <= sz(s) ? pos[term[i]] - len[i] : -1) << endl;
	}
}
