#include <vector>
#include <random>
#include <iostream>

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
pair<vi, vi> LIS(vi &a) {
	int n = sz(a);
	vi st, id, p(n);
	rep(i, 0, n) {
		int j = lower_bound(all(st), a[i]) - begin(st);
		if (j < sz(st)) {
			st[j] = a[i];
			p[i] = j > 0 ? id[j - 1] : -1;
			id[j] = i;
		} else {
			st.pb(a[i]);
			p[i] = j > 0 ? id[j - 1] : -1;
			id.pb(i);
		}
	}
	for (int i = sz(id) - 1; i > 0; i--) id[i - 1] = p[id[i]];
	return {id, p};
}
// end template //

int main() {
	cin.tie(0)->sync_with_stdio(0);
	int n;
	cin >> n;
	vi a(n);
	rep(i, 0, n) cin >> a[i];
	auto [id, p] = LIS(a);
	cout << sz(id) << endl;
}
