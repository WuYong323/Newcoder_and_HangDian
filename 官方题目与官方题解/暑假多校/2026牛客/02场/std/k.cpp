#include <bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n, m, T;
	cin >> n >> m >> T;

	vector<pair<int, int>> edges(m);
	for (auto &[u, v]: edges) {
		cin >> u >> v;
		u --, v --;
	}

	int k;
	cin >> k;

	vector<int> ends(n, 0), weights(m, 0);
	for (int i = 0; i < k; i ++) {
		int id; cin >> id; id --;
		ends[edges[id].first] = 1;
		ends[edges[id].second] = 1;
		cin >> weights[id];
	}

	vector<vector<int>> path(n), weight_path(n);

	for (int i = 0; i < m; i ++) {
		auto [u, v] = edges[i];
		if (!weights[i]) {
			path[u].emplace_back(v);
			path[v].emplace_back(u);
		}
		else {
			weight_path[u].emplace_back(i);
			weight_path[v].emplace_back(i);
		}
	}

	long long inf = 1e18;
	vector<vector<long long>> dis(n, vector<long long>(n, inf));

	for (int i = 0; i < n; i ++) {
		dis[i][i] = 0;
		queue<int> que;
		que.push(i);

		while (!que.empty()) {
			int u = que.front(); que.pop();
			for (auto &v: path[u]) {
				if (dis[i][v] == inf) {
					dis[i][v] = dis[i][u] + T;
					que.push(v);
				}
			}
		}
	}

	vector<int> special_nodes;
	for (int i = 0; i < n; i ++) {
		if (ends[i]) {
			special_nodes.emplace_back(i);
		}
	}

	vector<long long> ans(n, inf);
	vector<int> vis(n, 0);

	int q;
	cin >> q;

	while (q --) {
		int id; cin >> id; id --;
		cin >> weights[id];

		int l;
		cin >> l;

		while (l --) {
			int a, b;
			cin >> a >> b;
			a --, b --;

			vector<int> tmp = special_nodes;
			tmp.emplace_back(a);
			tmp.emplace_back(b);

			for (auto &x: tmp) ans[x] = inf, vis[x] = 0;

			int cur;

			vis[a] = 1;
			ans[a] = 0;
			cur = a;

			for (int i = 1; i <= 101; i ++) {
				int ncur = -1;
				long long ndis = inf;

				for (auto &v: tmp) {
					if (!vis[v]) {
						ans[v] = min(ans[v], ans[cur] + dis[cur][v]);
						if (ans[v] < ndis) {
							ncur = v;
							ndis = ans[v];
						}
					}
				}

				for (auto &eid: weight_path[cur]) {
					int v = edges[eid].first ^ edges[eid].second ^ cur;
					if (!vis[v]) {
						ans[v] = min(ans[v], ans[cur] + weights[eid]);
						if (ans[v] < ndis) {
							ncur = v;
							ndis = ans[v];
						}
					}
				}

				if (ncur == -1) break;

				cur = ncur;
				vis[cur] = 1;
			}

			cout << ans[b] << '\n';
		}
	}

	return 0;
}