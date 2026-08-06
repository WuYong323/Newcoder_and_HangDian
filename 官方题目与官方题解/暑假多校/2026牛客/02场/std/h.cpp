#include <bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int t;
	cin >> t;

	while (t --) {
		int n, x, y, c = 0;
		cin >> n >> x >> y;

        for (int i = 0; i < n; i ++) c += (x ^ y) >> i & 1;

		if (c & 1) cout << "No\n";
		else {
			cout << "Yes\n";

			int diff = x ^ y, k = c;
	
			vector<int> pairing(1 << n);
			for (int i = 0; i < (1 << n); i ++) {
				pairing[i] = i ^ 3;
			}

			pairing[0] = 0;
			pairing[3] = 3;

			for (int i = 2; i < k; i += 2) {
				int cur = (1 << i) - 1;
				pairing[cur] = cur << 1;
				pairing[cur << 1] = cur;

				pairing[(cur << 1) ^ 3] = cur << 2;
				pairing[cur << 2] = (cur << 1) ^ 3;
				
				pairing[(cur << 2) ^ 3] = (cur << 2) ^ 3;
			}

			vector<int> vals = {0};

			for (int i = 0; i < n; i ++) {
				if (diff >> i & 1) {
					int cur_len = vals.size();
					for (int j = 0; j < cur_len; j ++) {
						vals.emplace_back(vals[j] ^ (1 << i));
					}
				}
			}

			for (int i = 0; i < n; i ++) {
				if (!(diff >> i & 1)) {
					int cur_len = vals.size();
					for (int j = 0; j < cur_len; j ++) {
						vals.emplace_back(vals[j] ^ (1 << i));
					}
				}
			}

			for (int i = 0; i < (1 << n); i ++) {
				if (pairing[i] > i) {
					cout << (vals[i] ^ x) << ' ' << (vals[pairing[i]] ^ x) << '\n';
				}
			}
		}
	}

	return 0;
}