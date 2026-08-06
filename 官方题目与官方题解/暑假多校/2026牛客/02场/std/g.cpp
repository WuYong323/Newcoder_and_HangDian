#include <bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int M = 1e7, inf = 1e9;
	vector<int> pr(M + 1);
	iota(pr.begin(), pr.end(), 0);

	for (int i = 2; i <= M; i ++) {
		if (pr[i] == i) {
			for (int j = i; j <= M; j += i) {
				pr[j] = i;
			}
		}
	}

	auto coprime_cnt = [&] (int target, int up_bound) -> int {
		int tmp = target;
		vector<int> prime_factors;
		while (tmp > 1) {
			int p = pr[tmp];
			prime_factors.emplace_back(p);
			while (tmp % p == 0) tmp /= p;
		}

		int ans = 0;
		int k = prime_factors.size();

		for (int i = 0; i < (1 << k); i ++) {
			int cur = up_bound;
			for (int j = 0; j < k; j ++) {
				if (i >> j & 1) {
					cur /= prime_factors[j];
				}
			}
			if (__popcount(i) & 1) ans -= cur;
			else ans += cur;
		}

		return ans;
	};

	auto calc = [&] (int target, int to_calc) -> int {
		int smaller_prime = target - 1;
		while (pr[smaller_prime] != smaller_prime) smaller_prime --;

		if (to_calc < smaller_prime)
			return 2 * to_calc - coprime_cnt(target, to_calc);

		int ans = 2 * (smaller_prime - 1) - coprime_cnt(target, smaller_prime - 1);

		int gap = target - smaller_prime;
		vector<int> dp(gap + 1, inf);
		dp[0] = 0;

		for (int i = 1; i <= gap; i ++) {
			for (int j = 0; j < i; j ++) {
				dp[i] = min(dp[i], dp[j] + gcd(target - i, target - j));
			}
			if (target - i <= to_calc) ans += dp[i];
		}

		return ans;
	};

	int t;
	cin >> t;

	while (t --) {
		int l, r, n;
		cin >> l >> r >> n;
		cout << calc(n, r) - calc(n, l - 1) << '\n';
	}

	return 0;
}