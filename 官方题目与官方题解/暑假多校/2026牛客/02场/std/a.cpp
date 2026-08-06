#include <bits/stdc++.h>
using namespace std;

long long quickPow(long long base, long long power, long long mod) {
	if (power == 0) return 1 % mod;
	long long cur = quickPow(base, power / 2, mod);
	return power & 1 ? base * cur % mod * cur % mod : cur * cur % mod; 
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int t, M = 2e6 + 5, mod = 998244353;
	cin >> t;

	vector<int> f(M), g(M);

	f[0] = 1;
	for (int i = 1; i < M; i ++) {
		f[i] = 1ll * f[i - 1] * i % mod;
	}
	g[M - 1] = quickPow(f[M - 1], mod - 2, mod);
	for (int i = M - 1; i >= 1; i --) {
		g[i - 1] = 1ll * g[i] * i % mod;
	}

	auto comb = [&] (int x, int y) -> int {
		assert(x >= 0 && y >= 0 && y <= x);
		return 1ll * f[x] * g[y] % mod * g[x - y] % mod;
	};

	while (t --) {
		int n, m, p, q;
		cin >> n >> m >> p >> q;

		int ans = 2 * (n + m) - 2;
		int p1 = 1ll * p * quickPow(q, mod - 2, mod) % mod;
		int p2 = mod + 1 - p1;

		n --, m --;
		if (n == 0) ans += 1ll * m * p1 % mod, ans %= mod;
		else if (m == 0) ans += 1ll * n * p2 % mod, ans %= mod;
		else if (n > m) {
			int x = m, y = n - m, cur = 1, res = 0;
			for (int i = 0; i <= y; i ++) {
				res += 1ll * (y - i) * cur % mod * comb(x - 1 + i, i) % mod;
				res %= mod;
				cur = 1ll * cur * p1 % mod;
			}
			ans += res * quickPow(p2, x + 1, mod) % mod;
			ans %= mod;
		}
		else {
			int x = n, y = m - n, cur = 1, res = 0;
			for (int i = 0; i <= y; i ++) {
				res += 1ll * (y - i) * cur % mod * comb(x - 1 + i, i) % mod;
				res %= mod;
				cur = 1ll * cur * p2 % mod;
			}
			ans += res * quickPow(p1, x + 1, mod) % mod;
			ans %= mod;
		}
		cout << ans << '\n';
	}

	return 0;
}