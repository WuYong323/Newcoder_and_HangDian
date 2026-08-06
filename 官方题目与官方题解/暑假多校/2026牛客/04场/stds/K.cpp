#include <bits/stdc++.h>
using namespace std;
 
const int N = 3005;
const int MOD = 1000000007;
 
int n;
vector<int> g[N];
vector<int> f[N];
int sz[N];
int C[N][N];
 
void addmod(int &x, long long y) {
    x = (x + y) % MOD;
}
 
void dfs(int u, int fa) {
    sz[u] = 1;
 
    vector<int> dp(1, 1);
 
    for (int v : g[u]) {
        if (v == fa) continue;
 
        dfs(v, u);
 
        int m = sz[v];
 
        vector<int> suf(m + 1, 0);
        for (int i = m - 1; i >= 0; i--) {
            suf[i] = suf[i + 1] + f[v][i];
            if (suf[i] >= MOD) suf[i] -= MOD;
        }
 
        vector<int> h(m + 1, 0);
        h[0] = suf[0];
        for (int t = 1; t <= m; t++) {
            h[t] = suf[t - 1];
        }
 
        vector<int> ndp(dp.size() + m, 0);
 
        for (int a = 0; a < (int)dp.size(); a++) {
            if (dp[a] == 0) continue;
 
            for (int t = 0; t <= m; t++) {
                if (h[t] == 0) continue;
 
                long long ways = 1LL * dp[a] * h[t] % MOD * C[a + t][a] % MOD;
                addmod(ndp[a + t], ways);
            }
        }
 
        dp.swap(ndp);
        sz[u] += sz[v];
    }
 
    f[u] = move(dp);
}
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    cin >> n;
 
    for (int i = 1; i <= n - 1; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
 
    for (int i = 0; i <= n; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++) {
            C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
            if (C[i][j] >= MOD) C[i][j] -= MOD;
        }
    }
 
    dfs(1, 0);
 
    int ans = 0;
    for (int x : f[1]) {
        ans += x;
        if (ans >= MOD) ans -= MOD;
    }
 
    cout << ans << '\n';
 
    return 0;
}
