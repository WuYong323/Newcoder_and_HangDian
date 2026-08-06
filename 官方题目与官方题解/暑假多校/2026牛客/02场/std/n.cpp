#include <bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int t;
	cin >> t;

	while (t --) {
		int n, k;
		cin >> n >> k;

		vector<int> nums(n);
		long long tot = 0;
		for (auto &x: nums) cin >> x, tot += x;

		sort(nums.begin(), nums.end());

		vector<long long> acc(n + 1, 0);
		for (int i = 0; i < n; i ++) acc[i + 1] = acc[i] + nums[i];

		int left = (k - 1) / 2, right = k - left;
		long long max_diff = -1e18;

		for (int i = left; i + right <= n; i ++) {
			long long before = acc[left] + acc[i + right] - acc[i];
			long long after = (k & 1) ? 1ll * nums[i] * k : 1ll * (nums[i] + nums[i + 1]) * (k / 2);
			max_diff = max(max_diff, after - before);
		}

		cout << tot + max_diff << '\n';
	}

	return 0;
}