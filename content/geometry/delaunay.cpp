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

// Adapted from cp-algorithms contributors (principally SYury), CC BY-SA 4.0.
// https://cp-algorithms.com/geometry/delaunay.html
// Changes: indexed/recycled quad-edges, translated exact predicates, original IDs.

// begin template //
struct Delaunay {
	using I = __int128;
	struct Point { ll x,y; };
	struct Edge { int org=-1, next=0; };
	vector<Point> p;
	vi id, free;
	vector<Edge> e;
	vector<array<int,3>> triangles;
	vector<pair<int,int>> edges;
	static int rot(int a) { return (a&~3)|((a+1)&3); }
	int dest(int a) const { return e[a^2].org; }
	int prev(int a) const { return rot(e[rot(a)].next); }
	int left(int a) const { return rot(e[rot(a)^2].next); }
	I orient(int a,int b,int c) const {
		return I(p[b].x-p[a].x)*(p[c].y-p[a].y)
			-I(p[b].y-p[a].y)*(p[c].x-p[a].x);
	}
	bool circle(int a,int b,int c,int d) const {
		I x=p[a].x-p[d].x, y=p[a].y-p[d].y;
		I u=p[b].x-p[d].x, v=p[b].y-p[d].y;
		I s=p[c].x-p[d].x, t=p[c].y-p[d].y;
		return (x*x+y*y)*(u*t-v*s)-(u*u+v*v)*(x*t-y*s)
			+(s*s+t*t)*(x*v-y*u) > 0;
	}
	void splice(int a,int b) {
		int x=rot(e[a].next), y=rot(e[b].next);
		swap(e[x].next,e[y].next); swap(e[a].next,e[b].next);
	}
	int make(int a,int b) {
		int k;
		if (free.empty()) { k=sz(e); e.resize(k+4); }
		else { k=free.back(); free.pop_back(); }
		e[k]={a,k}; e[k+2]={b,k+2};
		e[k+1]={-1,k+3}; e[k+3]={-1,k+1}; return k;
	}
	void erase(int a) {
		splice(a,prev(a)); splice(a^2,prev(a^2));
		e[a].org=e[a^2].org=-1; free.push_back(a&~3);
	}
	int connect(int a,int b) {
		int c=make(dest(a),e[b].org);
		splice(c,left(a)); splice(c^2,b); return c;
	}
	pair<int,int> build(int l,int r) {
		if (r-l <= 3) {
			int a=make(l,l+1);
			if (r-l == 2) return {a,a^2};
			int b=make(l+1,l+2); splice(a^2,b);
			I s=orient(l,l+1,l+2);
			if (!s) return {a,b^2};
			int c=connect(b,a);
			return s>0 ? pair{a,b^2} : pair{c^2,c};
		}
		int m=(l+r)/2;
		auto [lo,li]=build(l,m); auto [ri,ro]=build(m,r);
		while (true) {
			if (orient(e[li].org,dest(li),e[ri].org)>0) li=left(li);
			else if (orient(e[ri].org,dest(ri),e[li].org)<0) ri=e[ri^2].next;
			else break;
		}
		int base=connect(ri^2,li);
		if (e[li].org==e[lo].org) lo=base^2;
		if (e[ri].org==e[ro].org) ro=base;
		auto valid=[&](int a) { return orient(e[base].org,dest(base),dest(a))<0; };
		while (true) {
			int a=e[base^2].next, b=prev(base);
			if (valid(a)) while (circle(dest(base),e[base].org,dest(a),dest(e[a].next))) {
				int t=e[a].next; erase(a); a=t;
			}
			if (valid(b)) while (circle(dest(base),e[base].org,dest(b),dest(prev(b)))) {
				int t=prev(b); erase(b); b=t;
			}
			if (!valid(a) && !valid(b)) break;
			if (!valid(a) || (valid(b) && circle(dest(a),e[a].org,e[b].org,dest(b))))
				base=connect(b,base^2);
			else base=connect(base^2,a^2);
		}
		return {lo,ro};
	}
	Delaunay(vector<Point> input) {
		int n=sz(input); id.resize(n); iota(all(id),0);
		sort(all(id),[&](int a,int b) {
			return tie(input[a].x,input[a].y)<tie(input[b].x,input[b].y);
		});
		for (int i : id) p.push_back(input[i]);
		if (n<2) return;
		build(0,n); vector<bool> seen(sz(e));
		for (int a=0; a<sz(e); a+=2) if (e[a].org>=0) {
			if (!(a&2)) edges.push_back({id[e[a].org],id[dest(a)]});
			if (seen[a]) continue;
			vi face; int b=a;
			do { seen[b]=true; face.push_back(e[b].org); b=left(b); } while (b!=a);
			if (sz(face)==3 && orient(face[0],face[1],face[2])>0)
				triangles.push_back({id[face[0]],id[face[1]],id[face[2]]});
		}
	}
};
// end template //
