#include <numeric>
#define main wavelet_main
#include "wavelet-matrix.cpp"
#undef main

// begin template //
WM wavPos(WM &wm) {
	WM pos;
	int n = sz(wm[0])-1;
	vi ord(n); iota(all(ord),0);
	for (int h = H-1; h >= 0; --h) {
		pos[h].resize(n);
		rep(i,0,n) {
			int k = ord[i], ones = wm[h][k];
			ord[i] = wm[h][k+1] > ones ? n-wm[h][n]+ones : k-ones;
			pos[h][i] = ord[i];
		}
	}
	return pos;
}

void rectOp(WM &wm, int l, int r, ll a, ll b, auto f) {
	auto go = [&](auto &&go, int h, int l, int r, ll lo, ll hi) -> void {
		if (l == r || b <= lo || hi <= a || a == b) return;
		if (h < H && a <= lo && hi <= b) { f(h,l,r); return; }
		auto [l0,r0,l1,r1] = getSplit(wm,h-1,l,r);
		ll mid = (lo+hi)/2;
		go(go,h-1,l0,r0,lo,mid);
		go(go,h-1,l1,r1,mid,hi);
	};
	go(go,H,l,r,0,1LL<<H);
}
// end template //
