#include <bits/stdc++.h>
using namespace std;
 
const int MAXN = 205;
const int MAXL = 65;
const int INF = 1e9;
 
int n, m, L;
int x[MAXL];
int dis[MAXN][MAXN];
 
int f[MAXN][MAXL][MAXL];
int g[MAXN][MAXL][MAXL];
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int T;
    cin >> T;
 
    while (T--) {
        cin >> n >> m >> L;
 
        vector<vector<int>> adj(n + 1);
 
        for (int i = 1; i <= m; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
 
        for (int i = 1; i <= L; i++) {
            cin >> x[i];
        }
 
        for (int s = 1; s <= n; s++) {
            for (int i = 1; i <= n; i++) {
                dis[s][i] = INF;
            }
 
            queue<int> q;
            dis[s][s] = 0;
            q.push(s);
 
            while (!q.empty()) {
                int u = q.front();
                q.pop();
 
                for (int v : adj[u]) {
                    if (dis[s][v] == INF) {
                        dis[s][v] = dis[s][u] + 1;
                        q.push(v);
                    }
                }
            }
        }
 
        for (int i = 1; i <= n; i++) {
            for (int l = 1; l <= L; l++) {
                for (int r = 1; r <= L; r++) {
                    f[i][l][r] = g[i][l][r] = INF;
                }
            }
        }
 
        vector<vector<int>> order(L + 1);
 
        for (int l = 1; l <= L; l++) {
            order[l].resize(n);
            iota(order[l].begin(), order[l].end(), 1);
 
            sort(order[l].begin(), order[l].end(), [&](int a, int b) {
                return dis[a][x[l]] < dis[b][x[l]];
            });
        }
 
        for (int len = 1; len <= L; len++) {
            for (int l = 1; l + len - 1 <= L; l++) {
                int r = l + len - 1;
 
                for (int i = 1; i <= n; i++) {
                    if (l == r && i == x[l]) {
                        f[i][l][r] = g[i][l][r] = 1;
                    }
 
                    for (int k = l; k < r; k++) {
                        if (dis[x[k]][x[k + 1]] == dis[x[k]][i] + dis[i][x[k + 1]]) {
                            f[i][l][r] = min(
                                f[i][l][r],
                                max(f[i][l][k], f[i][k + 1][r])
                            );
 
                            g[i][l][r] = min(
                                g[i][l][r],
                                max(f[i][l][k], g[i][k + 1][r])
                            );
                        }
                    }
                }
 
                for (int i : order[l]) {
                    for (int v : adj[i]) {
                        if (dis[i][x[l]] == dis[v][x[l]] + 1) {
                            g[i][l][r] = min(g[i][l][r], g[v][l][r] + 1);
 
                            if (dis[i][x[r]] == dis[v][x[r]] + 1) {
                                f[i][l][r] = min(f[i][l][r], f[v][l][r] + 1);
                            }
                        }
                    }
                }
            }
        }
 
        cout << g[x[1]][1][L] << '\n';
    }
 
    return 0;
}
