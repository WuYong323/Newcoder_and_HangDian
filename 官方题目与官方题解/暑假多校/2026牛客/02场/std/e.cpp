#include <bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int t;
	cin >> t;

	while (t --) {
		int n, m;
		cin >> n >> m;

		auto f = [&] (int i, int j) {return i * (m + 1) + j + 1;};

		vector<pair<int, int>> edges;
		auto draw_rectangle = [&] (auto &self, int xl, int xr, int yl, int yr) -> void {
			if (xr - xl <= 0 || yr - yl <= 0) return ;

			for (int i = xl; i < xr; i ++) {
				edges.emplace_back(f(i, yl), f(i + 1, yl));
				edges.emplace_back(f(i, yr), f(i + 1, yr));
			}

			for (int i = yl; i < yr; i ++) {
				edges.emplace_back(f(xl, i), f(xl, i + 1));
				edges.emplace_back(f(xr, i), f(xr, i + 1));
			}
			
			self(self, xl + 1, xr - 1, yl + 1, yr - 1);
		};

		if (min(n, m) & 1) draw_rectangle(draw_rectangle, 0, n, 0, m);
		else {
			if (n > m) {
				int x = n, y = m;
				while (x > 2 * y) {
					draw_rectangle(draw_rectangle, x - y, x, 0, y);
					x -= y + 1;
				}

				if (x == y || x == y + 1) draw_rectangle(draw_rectangle, 0, x, 0, y);
				else if (x % 2 == 0) {
					draw_rectangle(draw_rectangle, 0, y, 0, y);
					draw_rectangle(draw_rectangle, y + 1, x, 0, y);
				}
				else {
					int v = x / 2;
					if (!(v & 1)) v --;
					draw_rectangle(draw_rectangle, 0, v, 0, y);
					draw_rectangle(draw_rectangle, v + 1, x, 0, y);
				}
			}
			else {
				int x = n, y = m;
				while (y > 2 * x) {
					draw_rectangle(draw_rectangle, 0, x, y - x, y);
					y -= x + 1;
				}

				if (y == x || y == x + 1) draw_rectangle(draw_rectangle, 0, x, 0, y);
				else if (y % 2 == 0) {
					draw_rectangle(draw_rectangle, 0, x, 0, x);
					draw_rectangle(draw_rectangle, 0, x, x + 1, y);
				}
				else {
					int v = y / 2;
					if (!(v & 1)) v --;
					draw_rectangle(draw_rectangle, 0, x, 0, v);
					draw_rectangle(draw_rectangle, 0, x, v + 1, y);
				}
			}
		}

		cout << edges.size() << '\n';
		for (auto &[u, v]: edges) cout << u << ' ' << v << '\n';
	}

	return 0;
}