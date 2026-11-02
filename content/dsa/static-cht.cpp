#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numeric>
#include <random>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
using ll = long long;
using ld = long double;
using vi = vector<int>;
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)

// begin template //
struct StaticCHT {
	struct Line { ll a, b; ld at(ld x) const { return a*x+b; } };
	vector<Line> h;
	vector<ld> start;
	StaticCHT(vector<Line> v) {
		sort(all(v), [](Line a, Line b) {
			return tie(a.a,a.b) < tie(b.a,b.b);
		});
		for (auto l : v) {
			ld x = -numeric_limits<ld>::infinity();
			while (!h.empty()) {
				auto p = h.back();
				if (l.a != p.a) {
					x = ((ld)p.b-l.b)/((ld)l.a-p.a);
					if (x > start.back()) break;
				}
				h.pop_back(); start.pop_back();
			}
			if (h.empty()) x = -numeric_limits<ld>::infinity();
			h.push_back(l); start.push_back(x);
		}
	}
	int index(ld x) const {
		return upper_bound(all(start),x)-start.begin()-1;
	}
	ld query(ld x) const { return h[index(x)].at(x); }
};
// end template //
