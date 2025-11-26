#include <bits/stdc++.h>

using namespace std;

using i64 = int64_t;
using i32 = int32_t;
using i128 = __int128;
using VI = vector<i32>;
using VVI = vector<VI>;
using PII = pair<i32, i32>;
using VII = vector<PII>;
using VVII = vector<VII>;

// debug stuff, no need to type at first
void PRINT(auto x) { cerr << '\t' << x << endl; }
void PRINT(const char *name, auto... x) { 
	cerr << "DEBUG> "<< name << " = [" << endl;
	(PRINT(x), ...); 
	cerr << ']' << endl;
}
#define DEBUG(...) PRINT(#__VA_ARGS__, __VA_ARGS__)

// #define int i64

i32 main() {
	cin.tie(0)->sync_with_stdio(0);
	return 0;
}
