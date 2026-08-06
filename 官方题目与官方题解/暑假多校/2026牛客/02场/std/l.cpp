#include <bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n, mod = 998244353;
	cin >> n;

	vector<int> perm(n);
	for (auto &x: perm) cin >> x;

	bool flg = true;
	for (int i = 0; i < n; i ++)
		if (perm[i] != i + 1)
			flg = false;

	if (flg) {
		int ans = 1;
		for (int i = 1; i <= n; i ++)
			ans = 1ll * ans * i % mod;
		return cout << ans << '\n', 0;
	}

	vector<int> msk(n, 0);

	for (int i = 0; i < n; i ++) {
		for (int j = i + 1; j < n; j ++) {
			if (perm[i] > perm[j]) {
				msk[i] |= 1 << j;
			}
		}
	}

	vector<int> dp(1 << n, 0);
	dp[0] = 1;

	for (int i = 0; i < (1 << n); i ++) {
		for (int j = 0; j < n; j ++) {
			if (!(i >> j & 1) && ((msk[j] & i) == msk[j])) {
				dp[i ^ (1 << j)] += dp[i];
				dp[i ^ (1 << j)] %= mod;
			}
		}
	}

	cout << 2 * dp.back() % mod << '\n';

	return 0;
}