#include <bits/stdc++.h>
using namespace std;
 
using ll = long long;
 
const ll INF = (1LL << 62);
 
struct Edge {
    int u, v;
    ll w;
};
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int T;
    cin >> T;
 
    cout << fixed << setprecision(12);
 
    while (T--) {
        int n, m, q;
        cin >> n >> m >> q;
 
        vector<Edge> edges(m + 1);
        vector<vector<pair<int, ll>>> g(n + 1);
 
        for (int i = 1; i <= m; i++) {
            int u, v;
            ll w;
            cin >> u >> v >> w;
            edges[i] = {u, v, w};
            g[u].push_back({v, w});
        }
 
        vector<vector<ll>> dis(n + 1, vector<ll>(n + 1, INF));
 
        auto dijkstra = [&](int s) {
            priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
 
            dis[s][s] = 0;
            pq.push({0, s});
 
            while (!pq.empty()) {
                auto [d, x] = pq.top();
                pq.pop();
 
                if (d != dis[s][x]) continue;
 
                for (auto [y, w] : g[x]) {
                    if (dis[s][y] > d + w) {
                        dis[s][y] = d + w;
                        pq.push({dis[s][y], y});
                    }
                }
            }
        };
 
        for (int s = 1; s <= n; s++) {
            dijkstra(s);
        }
 
        while (q--) {
            int id;
            ll x;
            cin >> id >> x;
 
            int a = edges[id].u;
            int b = edges[id].v;
 
            long double ans = 0;
 
            for (int e = 1; e <= m; e++) {
                int u = edges[e].u;
                int v = edges[e].v;
 
                ll den = edges[e].w;
                if (e == id) den = x;
 
                ll nd = dis[v][u];
 
                if (dis[v][a] < INF && dis[b][u] < INF) {
                    nd = min(nd, dis[v][a] + x + dis[b][u]);
                }
 
                ans = max(ans, (long double)nd / (long double)den);
            }
 
            cout << (double)ans << '\n';
        }
    }
 
    return 0;
}
