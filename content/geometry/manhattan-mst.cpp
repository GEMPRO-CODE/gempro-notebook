// Adapted from KACTL, chilli and Takanori MAEHARA; CC0.
// https://github.com/kth-competitive-programming/kactl/blob/main/content/geometry/ManhattanMST.h
#include <algorithm>
#include <complex>
#include <map>
#include <numeric>
#include <tuple>
#include <vector>
using namespace std;
using ll = long long;
using vi = vector<int>;
using Pt = complex<ll>;
#define xx real()
#define yy imag()
#define sz(x) (int)(x).size()
#define all(x) begin(x), end(x)
#define rep(i,a,b) for (int i = (a); i < (b); ++i)

// begin template //
vector<tuple<ll,int,int>> manhattanMST(vector<Pt> p) {
	vi id(sz(p)); iota(all(id),0);
	vector<tuple<ll,int,int>> edges;
	rep(k,0,4) {
		sort(all(id), [&](int i,int j) {
			return p[i].xx+p[i].yy < p[j].xx+p[j].yy;
		});
		map<ll,int> sweep;
		for (int i : id) {
			for (auto it = sweep.lower_bound(-p[i].yy); it != sweep.end();) {
				int j = it->second;
				Pt d = p[i]-p[j];
				if (d.yy > d.xx) break;
				edges.emplace_back(d.xx+d.yy,i,j);
				it = sweep.erase(it);
			}
			sweep[-p[i].yy] = i;
		}
		for (Pt &v : p) v = k&1 ? Pt(-v.xx,v.yy) : Pt(v.yy,v.xx);
	}
	return edges;
}
// end template //
