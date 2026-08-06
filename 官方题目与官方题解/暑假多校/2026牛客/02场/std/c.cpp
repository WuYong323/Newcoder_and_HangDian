#include <bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int t, inf = 1e9;
	cin >> t;

	while (t --) {
		int k, x1, y1, x2, y2;
		cin >> k >> x1 >> y1 >> x2 >> y2;

		if (max(x2, y2) < k) {
			int dx = x2 - x1, dy = y2 - y1;
			cout << (dx - 1) / (dy + 1) + 1 << ' ' << dx << '\n';
		}
		else if (max(x1, y1) >= k) {
			y1 = max(x1, y1);
			int dx = x2 - x1, dy = y2 - y1;
			if (dy < 0) cout << dx << ' ' << dx << '\n';
			else cout << (dx - 1) / (dy + 1) + 1 << ' ' << min(dx, max(2, x2 - y2 + 1)) << '\n';
		}
		else if (max(x2, y2) == k && min(x2, y2) <= k - 2) {
			if (x2 == k) {
				int dx = x2 - x1, dy = y2 - y1;
				cout << (dx - 1) / (dy + 1) + 1 << ' ' << dx << '\n';
			}
			else {
				int dx = x2 - x1, dy = y2 - y1 - 1;
				cout << (dx - 1) / (dy + 1) + 1 << ' ' << dx << '\n';
			}
		}
		else if (x2 == k - 1) {
			y2 = k - 1;
			int dx = x2 - x1, dy = y2 - y1;
			cout << (dx - 1) / (dy + 1) + 1 << ' ' << dx << '\n';
		}
		else if (y2 == k - 1) {
			int dx = x2 - x1, dy = y2 - y1;
			cout << max(x2 - y2, (dx - 1) / (dy + 1) + 1) << ' ' << dx << '\n';
		}
		else {
			int mi = inf, ma = 0;
			if (y2 >= k) {
				int dx1 = (k - 1) - x1, dy1 = (k - 1) - y1;
				int dx2 = x2 - (k - 1), dy2 = y2 - k;
				mi = min(mi, max((dx1 - 1) / (dy1 + 1) + 1, (dx2 - 1) / (dy2 + 1) + 1));
				ma = max(ma, max(dx1, min(dx2, max(2, x2 - y2 + 1))));
			}
			if (x2 >= k) {
				int dx1 = k - x1, dy1 = (k - 1) - y1;
				int dx2 = 0, dy2 = 0;
				if (x2 >= k)
					dx2 = x2 - k, dy2 = max(0, y2 - k);
				mi = min(mi, max((dx1 - 1) / (dy1 + 1) + 1, (dx2 - 1) / (dy2 + 1) + 1));
				ma = max(ma, max(dx1, min(dx2, max(2, x2 - y2 + 1))));
			}
			cout << mi << ' ' << ma << '\n';
		}
	}

	return 0;
}
