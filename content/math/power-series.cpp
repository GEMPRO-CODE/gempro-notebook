// https://cp-algorithms.com/algebra/polynomial.html
#define main ntt_main
#include "../dsa/ntt.cpp"
#undef main

// begin template //
vector<ll> fpsInv(vector<ll> a, int n) {
	vector<ll> b = {powm(a[0],mod-2)};
	while (sz(b) < n) {
		int m = min(n,2*sz(b));
		vector<ll> cut(a.begin(),a.begin()+min(m,sz(a)));
		auto c = conv(cut,b); c.resize(m);
		for (ll &x : c) x = (mod-x) % mod;
		c[0] = (c[0]+2) % mod;
		b = conv(b,c); b.resize(m);
	}
	b.resize(n); return b;
}
vector<ll> fpsLog(vector<ll> a, int n) {
	a.resize(n);
	vector<ll> d(max(0,n-1)), inv(n,1);
	rep(i,1,n) d[i-1] = a[i]*i % mod;
	auto b = fpsInv(a,n), c = conv(d,b);
	c.resize(n);
	for (int i = 2; i < n; ++i) inv[i] = mod - (mod/i)*inv[mod%i] % mod;
	for (int i = n-1; i > 0; --i) c[i] = c[i-1]*inv[i] % mod;
	c[0] = 0; return c;
}
vector<ll> fpsExp(vector<ll> a, int n) {
	a.resize(n);
	vector<ll> b = {1};
	while (sz(b) < n) {
		int m = min(n,2*sz(b));
		auto c = fpsLog(b,m);
		rep(i,0,m) c[i] = (a[i]-c[i]+mod) % mod;
		c[0] = (c[0]+1) % mod;
		b = conv(b,c); b.resize(m);
	}
	return b;
}
// end template //
