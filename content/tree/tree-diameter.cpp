#include <vector>
#include <random>
#include <iostream>
#include <algorithm>
#include <map>

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
vi treeDiam(int n, vector<vi> &g) {
	int k;
	vi d(n), t(n), s(n), p(n), dia;
	rep(a,0,2) {
		k = 1, p[t[0]] = -1, s.assign(n, 0);
		for (int i: t) {
			s[i] = 1;
			for (int j: g[i]) if (!s[j]) d[j] = d[i] + 1, t[k++] = j, p[j] = i;
		}
		t[0] = max_element(all(d)) - begin(d);
	}
	for (int i = t[0]; i != -1; i = p[i]) dia.pb(i);
	return dia;
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n;
	cin >> n;
	vector<vi> g(n);
	rep(i, 1, n) {
		int a, b;
		cin >> a >> b;
		a--,b--;
		g[a].pb(b),g[b].pb(a);
	}
	auto dia = treeDiam(n, g);
	cout << sz(dia) - 1 << endl;
}
