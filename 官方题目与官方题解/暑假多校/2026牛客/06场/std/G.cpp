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
    queue<int> q;
    vector<int> win(n), cnt(n), s(n);
    while (k--) {
        int u;
        cin >> u;
        q.push(u - 1);
        win[u - 1] = 1;
        s[u - 1] = 1;
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto v : g[u]) if (!win[v]) {
            cnt[v]++;
            if (cnt[v] == 2) {
                win[v] = 1;
                q.push(v);
            }
        }
    }
    vector<int> ans;
    for (int u = 0; u < n; u++) if (!s[u]) {
        int c = win[u];
        for (auto v : g[u]) c |= win[v];
        if (c) ans.push_back(u);
    }
    cout << ans.size() << "\n";
    for (auto v : ans) cout << v + 1 << " ";
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}