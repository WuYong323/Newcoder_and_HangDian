#include <bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int t;
	cin >> t;

	while (t --) {
		int n;
		cin >> n;

		vector<int> nums(n);
		for (auto &v: nums) cin >> v;

		int total_xor = 0;
		for (auto &v: nums) total_xor ^= v;

		vector<int> xor_base(30, 0);

		for (auto &x: nums) {
			x &= ~total_xor;
			for (int i = 29; i >= 0; i --) {
				if (x >> i & 1) {
					if (xor_base[i]) x ^= xor_base[i];
					else {
						xor_base[i] = x;
						break;
					}
				}
			}
		}

		int maximized = 0;
		for (int i = 29; i >= 0; i --) {
			if ((maximized ^ xor_base[i]) > maximized) {
				maximized ^= xor_base[i];
			}
		}

		cout << total_xor + 2 * maximized << '\n';
	}

	return 0;
}