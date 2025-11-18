#include <vector>

using namespace std;

using VI = vector<int>;

// begin template //
VI subsetZeta(VI a) {
	int n = a.size();
	for (int j = 1; j < n; j <<= 1)
		for (int i = 1; i < n; i++)
			if (i & j) a[i] += a[i ^ j];
	return a;
}
VI subsetMobius(VI a) {
	int n = a.size();
	for (int j = 1; j < n; j <<= 1)
		for (int i = 1; i < n; i++)
			if (i & j) a[i] -= a[i ^ j];
	return a;
}
VI supersetZeta(VI a) {
	int n = a.size();
	for (int j = 1; j < n; j <<= 1)
		for (int i = 1; i < n; i++)
			if (i & j) a[i ^ j] += a[i];
	return a;
}
VI supersetMobius(VI a) {
	int n = a.size();
	for (int j = 1; j < n; j <<= 1)
		for (int i = 1; i < n; i++)
			if (i & j) a[i ^ j] -= a[i];
	return a;
}
// end template //
