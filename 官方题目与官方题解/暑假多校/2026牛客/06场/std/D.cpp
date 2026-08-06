#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n, m, k;
    cin >> n >> m >> k;
    vector<vector<int>> g(n);
    while (m--) {
        int u, v;
        cin >> u >> v;
        u--;
        v--;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    vector dis(n, array<int, 2>{-1, -1});
    queue<pair<int, int>> q;
    q.emplace(0, 0);
    dis[0][0] = 0;
    while (!q.empty()) {
        auto [u, p] = q.front();
        q.pop();
        for (auto v : g[u]) {
            if (dis[v][p ^ 1] == -1) {
                dis[v][p ^ 1] = dis[u][p] + 1;
                q.emplace(v, p ^ 1);
            }
        }
    }
    for (int i = 0; i < n; i++) {
        int mn = 2e9 + 10;
        for (int j = 0; j < 2; j++) if (dis[i][j] != -1) {
            int q = dis[i][j] / k;
            for (int s = q; s <= q + 2; s++) if ((s * k) % 2 == j && s * k >= dis[i][j]) mn = min(mn, s * k);
        }
        if (mn == 2e9 + 10) cout << -1 << " ";
        else cout << mn << " ";
    }
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}