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

#include <unordered_set>

// begin template //
struct Point3 {
	ll x,y,z;
	Point3 operator-(Point3 b) const { return {x-b.x,y-b.y,z-b.z}; }
};
__int128 volume6(Point3 a, Point3 b, Point3 c, Point3 d) {
	b = b-a; c = c-a; d = d-a;
	return (__int128)b.x*( (__int128)c.y*d.z-(__int128)c.z*d.y)
		- (__int128)b.y*( (__int128)c.x*d.z-(__int128)c.z*d.x)
		+ (__int128)b.z*( (__int128)c.x*d.y-(__int128)c.y*d.x);
}
vector<array<int,3>> hull3D(const vector<Point3> &p) {
	int n = sz(p);
	if (n < 4) return {};
	vi id(n); iota(all(id),0);
	auto noncollinear = [&](int a,int b,int c) {
		auto u = p[b]-p[a], v = p[c]-p[a];
		return (__int128)u.x*v.y != (__int128)u.y*v.x ||
			(__int128)u.x*v.z != (__int128)u.z*v.x ||
			(__int128)u.y*v.z != (__int128)u.z*v.y;
	};
	int b = 1;
	while (b < n && p[b].x == p[0].x && p[b].y == p[0].y && p[b].z == p[0].z) ++b;
	if (b == n) return {};
	swap(id[1],id[b]); b = 2;
	while (b < n && !noncollinear(id[0],id[1],id[b])) ++b;
	if (b == n) return {};
	swap(id[2],id[b]); b = 3;
	while (b < n && !volume6(p[id[0]],p[id[1]],p[id[2]],p[id[b]])) ++b;
	if (b == n) return {};
	swap(id[3],id[b]);
	vector<array<int,3>> f;
	rep(i,0,4) rep(j,i+1,4) rep(k,j+1,4) {
		array<int,3> t = {id[i],id[j],id[k]};
		if (volume6(p[t[0]],p[t[1]],p[t[2]],p[id[6-i-j-k]]) > 0) swap(t[1],t[2]);
		f.push_back(t);
	}
	rep(k,4,n) {
		unordered_set<uint64_t> boundary;
		vector<array<int,3>> keep;
		for (auto t : f) {
			if (volume6(p[t[0]],p[t[1]],p[t[2]],p[id[k]]) <= 0) {
				keep.push_back(t); continue;
			}
			rep(j,0,3) {
				uint64_t a = t[j], b = t[(j+1)%3];
				if (!boundary.erase((b<<32)|a)) boundary.insert((a<<32)|b);
			}
		}
		for (auto e : boundary)
			keep.push_back({int(e>>32),
				int(uint32_t(e)),id[k]});
		f.swap(keep);
	}
	return f;
}
// end template //
