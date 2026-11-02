#include <vector>
#include <utility>
using namespace std;

// begin template //
template<class S, S (*op)(S,S), S (*e)()>
struct MonoidDeque {
	vector<pair<S,S>> t[2];
	int size() const { return t[0].size()+t[1].size(); }
	S agg(int s) const { return t[s].empty() ? e() : t[s].back().second; }
	void push(int s, S x) {
		S a = agg(s);
		t[s].push_back({x,s ? op(a,x) : op(x,a)});
	}
	void pop(int s) {
		if (t[s].empty()) {
			vector<pair<S,S>> v;
			v.swap(t[1-s]);
			int n = v.size(), k = (n+1)/2;
			for (int i = k-1; i >= 0; --i) push(s,v[i].first);
			for (int i = k; i < n; ++i) push(1-s,v[i].first);
		}
		t[s].pop_back();
	}
	void push_front(S x) { push(0,x); }
	void push_back(S x) { push(1,x); }
	void pop_front() { pop(0); }
	void pop_back() { pop(1); }
	S front() const {
		return t[0].empty() ? t[1].front().first : t[0].back().first;
	}
	S back() const {
		return t[1].empty() ? t[0].front().first : t[1].back().first;
	}
	S query() const { return op(agg(0),agg(1)); }
};
// end template //
