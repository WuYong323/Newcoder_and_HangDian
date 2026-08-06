#include <bits/stdc++.h>

const int N = 2e5 + 5;

int n, m, a[N], anc[N], eu[N], ev[N], dis[N];
std::vector<int> e[N];

int find(int u) {
    return anc[u] == u ? u : anc[u] = find(anc[u]);
}

void run() {
    scanf("%d%d", &n, &m);
    for (int u = 1; u <= n; u++) {
        scanf("%d", &a[u]);
        anc[u] = u;
        dis[u] = 1e9;
        e[u].clear();
    }
    for (int i = 0; i < m; i++) {
        int u, v;
        scanf("%d%d", &u, &v);
        eu[i] = u;
        ev[i] = v;
        if (a[u] == a[v] && find(u) != find(v))
            anc[find(u)] = find(v);
    }
    int s = -1, t;
    for (int i = 0; i < m; i++) {
        int u = eu[i], v = ev[i];
        if (a[u] != a[v])
            if (find(u) == find(v)) {
                s = u;
                t = v; 
                continue;
            } else
                anc[find(u)] = find(v);
        e[u].push_back(v);
        e[v].push_back(u);
    }
    if (s == -1) {
        printf("No\n");
        return;
    }
    std::queue<int> que;
    dis[s] = 0;
    que.push(s);
    while (!que.empty()) {
        int u = que.front();
        que.pop();
        for (auto v : e[u])
            if (dis[v] > dis[u] + 1) {
                dis[v] = dis[u] + 1;
                que.push(v);
            }
    }
    printf("Yes\n");
    for (int i = 0; i < m; i++) {
        int u = eu[i], v = ev[i];
        if ((dis[u] < dis[v] || (dis[u] == dis[v] && u > v)) && !(u == s && v == t))
            std::swap(u, v);
        printf("%d %d\n", u, v);
    }
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) run();
    return 0;
}
