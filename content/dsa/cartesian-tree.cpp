#include <vector>

using namespace std;

// begin template //
vector<int> cartTree(int n, auto le) {
	int last;
	vector<int> st, par(n);
	for (int i = 0; i < n; i++){ 
		par[i] = i, last = -1;
		while (st.size() && le(par[i], st.back())) {
			if (last != -1) par[last] = st.back();
			last = st.back();
			st.pop_back();
		}
		if (last != -1) par[last] = i;
		st.push_back(i);
	}
	while (st.size() > 1) {
		last = st.back();
		st.pop_back();
		par[last] = st.back();
	}
	return par;
}
// end template //
