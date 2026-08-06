#include <bits/stdc++.h>
using namespace std;
 
const int N = 505;
int dp0[N][N], dp1[N][N], comb[N][N], pows[N][N], mod;
 
void add(int &x, int y) {
	x += y;
	if (x >= mod) x -= mod;
}
 
long long quickPow(long long base, long long power, long long mod) {
	if (power == 0) return 1 % mod;
	long long cur = quickPow(base, power / 2, mod);
	return power & 1 ? base * cur % mod * cur % mod : cur * cur % mod; 
}
 
int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
 
	int N, P;
	cin >> N >> P >> mod;
 
	for (int i = 0; i <= N; i ++) {
		comb[i][0] = 1;
		comb[i][i] = 1;
		for (int j = 1; j < i; j ++) {
			comb[i][j] = (comb[i - 1][j - 1] + comb[i - 1][j]) % mod;
		}
	}
 
	for (int i = 1; i <= N; i ++) {
		pows[i][0] = 1;
		for (int j = 1; j <= N; j ++) {
			pows[i][j] = 1ll * pows[i][j - 1] * i % mod;
		}
	}
 
	dp0[0][0] = 1;
 
	for (int i = 0; i <= N; i ++) {
		for (int j = 0; j <= i; j ++) {
			for (int k = 0; k <= N - i; k ++) {
				int method = 1ll * comb[i + k][k] * pows[k][j] % mod;
				int v0 = 1ll * dp0[i][j] * method % mod;
				add(dp0[i + k][k], v0);
				add(dp1[i + k][k], 1ll * (k ^ P) % P * v0 % mod);
				add(dp1[i + k][k], 1ll * P * P * dp1[i][j] % mod * method % mod);
			}
		}
	}
 
	for (int n = 3; n <= N; n ++) {
		for (int i = 2; i <= n; i ++) {
			int c0 = 0, c1 = 0, base = quickPow(P, i - 1, mod);
			for (int j = 0; j <= n - i; j ++) {
				int v0 = 1ll * dp0[n - i][j] * pows[i][j] % mod;
				add(c0, v0);
				add(c1, 1ll * ((i - 2) ^ P) % P * v0 % mod);
				add(c1, 1ll * P * P * dp1[n - i][j] % mod * pows[i][j] % mod);
			}
			cout << 1ll * c1 * quickPow(c0, mod - 2, mod) % mod * base % mod << " \n"[i == n];
		}
	}
 
	return 0;
}