// Eertree: Rubinchik and Shur, https://arxiv.org/abs/1506.04862
#include <array>
#include <vector>
using namespace std;
using vi = vector<int>;
#define sz(x) (int)(x).size()

// begin template //
struct Eertree {
	struct Node {
		int len, link = 0, cnt = 0, end = -1;
		array<int,26> to{};
		Node(int len): len(len) {}
	};
	vector<Node> t = {Node(-1),Node(0)};
	vi s{-1}; int last = 1;
	int getLink(int v,int c) {
		while (s[sz(s)-2-t[v].len] != c) v = t[v].link;
		return v;
	}
	int add(char ch) {
		int c = ch-'a'; s.push_back(c);
		int v = getLink(last,c);
		if (!t[v].to[c]) {
			int u = sz(t); t.emplace_back(t[v].len+2);
			t[u].link = t[u].len == 1 ? 1 : t[getLink(t[v].link,c)].to[c];
			t[u].end = sz(s)-2;
			t[v].to[c] = u;
		}
		last = t[v].to[c]; ++t[last].cnt;
		return last;
	}
	vi occurrences() const {
		vi count;
		for (auto &v : t) count.push_back(v.cnt);
		for (int v = sz(t)-1; v >= 2; --v) count[t[v].link] += count[v];
		return count;
	}
};
// end template //
