// Bostan and Mori: https://arxiv.org/abs/2008.08822
#define main ntt_main
#include "../dsa/ntt.cpp"
#undef main

// begin template //
ll bostanMori(vector<ll> p, vector<ll> q, ll k) {
	while (k) {
		auto neg = q;
		for (int i = 1; i < sz(q); i += 2) neg[i] = (mod - neg[i]) % mod;
		auto a = conv(p,neg), b = conv(q,neg);
		p.clear(), q.clear();
		for (int i = k & 1; i < sz(a); i += 2) p.pb(a[i]);
		for (int i = 0; i < sz(b); i += 2) q.pb(b[i]);
		k >>= 1;
	}
	return p.empty() ? 0 : p[0] * powm(q[0],mod-2) % mod;
}
ll fastRec(vector<ll> s, vector<ll> c, ll k) {
	int d = sz(c);
	vector<ll> q(d+1,1);
	rep(i,0,d) q[i+1] = (mod-c[i]) % mod;
	auto p = conv(s,q);
	p.resize(d);
	return bostanMori(p,q,k);
}
// end template //
