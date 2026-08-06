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

		vector<vector<pair<int, int>>> path(n);
		int bound = 0;

		for (int i = 0; i < n - 1; i ++) {
			int u, v, w;
			cin >> u >> v >> w;
			u --, v --;
			path[u].emplace_back(v, w);
			path[v].emplace_back(u, w);
			bound = max(w * 2, bound);
		}

		vector<int> ans(n, bound);

		auto dfs = [&] (auto &self, int u, int p) -> vector<int> {
			vector<int> dp(bound, 0);
			for (auto &[v, w]: path[u]) {
				if (v != p) {
					auto ndp = self(self, v, u);
					vector<int> tmp(bound, bound);
					for (int i = 0; i < bound; i ++) {
						tmp[max(0, i - w)] = min(tmp[max(0, i - w)], ndp[i] + w);
						if (i + w < bound) tmp[i + w] = min(tmp[i + w], max(0, ndp[i] - w));
					}
					for (int i = 1; i < bound; i ++) tmp[i] = min(tmp[i], tmp[i - 1]);
					for (int i = 0; i < bound; i ++) dp[i] = max(dp[i], tmp[i]);
				}
			}

			for (int i = 0; i < bound; i ++) ans[u] = min(ans[u], dp[i] + i);

			return dp;
		};

		dfs(dfs, 0, -1);

		for (int i = 0; i < n; i ++) cout << ans[i] << (i < n - 1 ? ' ' : '\n');
	}

	return 0;
}