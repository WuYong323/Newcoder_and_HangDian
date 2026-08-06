#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 7;
const int M = 1 << 22;
const int mod = 998244353;
int n, m, k;
vector<pair<int, int> > G[N];
int d[N];
int f[M], g[M], sz[M];

void dfs(int u, int pre) {
    for (auto [v, id]: G[u]) {
        if (v == pre)
            continue;
        if (id != -1) {
            d[v] = d[u] | (1 << id);
        } else d[v] = d[u];
        dfs(v, u);
    }
}

int kuasum(int a, int b) {
    int ans = 1;
    while (b) {
        if (b & 1)
            ans = 1ll * ans * a % mod;
        b >>= 1;
        a = 1ll * a * a % mod;
    }
    return ans;
}

signed main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    cin >> n >> m >> k;
    int all = (1 << m) - 1;
    for (int i = 1; i < (1 << m); i++)
        sz[i] = sz[i >> 1] + (i & 1);
    vector<pair<int, int> > edge(n - 1);
    vector<int> vis(n - 1, -1);
    for (auto &[u, v]: edge)
        cin >> u >> v;
    for (int i = 0; i < m; i++) {
        int x;
        cin >> x;
        x--;
        vis[x] = i;
    }
    for (int i = 0; i < n - 1; i++) {
        auto [u, v] = edge[i];
        G[u].push_back({v, vis[i]});
        G[v].push_back({u, vis[i]});
    }
    dfs(1, 0);
    int ans = 0;
    for (int i = 1; i <= k; i++) {
        int s, t;
        cin >> s >> t;
        int nb = d[s] ^ d[t];
        f[nb]++;
    }
    for (int i = 0; i <= all; i++)
        g[i] = 1;
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < (1 << m); i++) {
            if (i >> j & 1)
                f[i] = (f[i] + f[i ^ (1 << j)]) % mod;
        }
    }
    int inv = 1;
    for (int i = 1; i <= 22; i++) {
        inv = 1ll * inv * kuasum(i, mod - 2) % mod;
        for (int j = 0; j <= all; j++)
            g[j] = 1ll * g[j] * f[j] % mod;
        int ans = 0;
        for (int j = 0; j <= all; j++) {
            if (sz[j ^ all] & 1)
                ans = (ans + mod - g[j]) % mod;
            else
                ans = (ans + g[j]) % mod;
        }
        if (ans) {
            cout << i << " " << 1ll * ans * inv % mod << "\n";
            return 0;
        }
    }
    cout << "-1\n";
    return 0;
}
