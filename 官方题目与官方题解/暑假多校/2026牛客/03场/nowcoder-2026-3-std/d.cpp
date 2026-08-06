// Longest Run of Ones -- MAIN solution.
//
// Model. We must pick a topological order maximizing the longest block of
// consecutive 1s. A consecutive block in a topo order is exactly an
// "order-convex" set of vertices (closed under "between" w.r.t. reachability),
// and the best block is the largest such set consisting only of 1-labeled
// vertices. Assign each vertex a group grp(v) in {0,1,2}: 0 = before block,
// 1 = inside block, 2 = after block. Feasibility <=> every edge u->v has
// grp(u) <= grp(v), and 0-labeled vertices may not take group 1. Maximize the
// number of group-1 vertices.
//
// Threshold encoding: b_v = [grp(v) >= 1], c_v = [grp(v) >= 2], so
// [grp(v) == 1] = b_v - c_v. Maximizing sum (b_v - c_v) under the implication
// constraints is a maximum-weight closed subgraph (weights +1 on b_v, -1 on
// c_v), solved by min cut:
//   s -> b_v (cap 1), c_v -> t (cap 1),
//   c_v -> b_v (inf), edge u->v gives b_u->b_v and c_u->c_v (inf),
//   if a_v == 0 then b_v -> c_v (inf).
// Answer L = n - maxflow. Reading the source side of the min cut gives grp(v),
// from which we output one optimal order (group 0, then 1, then 2, each
// topologically sorted).
//
// Complexity: maxflow <= n (all source edges are unit), so at most n
// augmenting paths; Dinic runs in O(n * (n + m)) per test.

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Dinic {
    struct E { int to, rev; long long cap; };
    vector<vector<E>> g;
    vector<int> level, it;
    int n;
    Dinic(int n) : g(n), level(n), it(n), n(n) {}
    void addEdge(int u, int v, long long c) {
        g[u].push_back({v, (int)g[v].size(), c});
        g[v].push_back({u, (int)g[u].size() - 1, 0});
    }
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto& e : g[u])
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
        }
        return level[t] >= 0;
    }
    long long dfs(int u, int t, long long f) {
        if (u == t) return f;
        for (int& i = it[u]; i < (int)g[u].size(); i++) {
            E& e = g[u][i];
            if (e.cap > 0 && level[e.to] == level[u] + 1) {
                long long d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    g[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }
    long long maxflow(int s, int t) {
        long long flow = 0;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            long long f;
            while ((f = dfs(s, t, (long long)4e18)) > 0) flow += f;
        }
        return flow;
    }
    vector<char> minCutSourceSide(int s) {
        // Nodes reachable from s in the residual graph = source side of min cut.
        vector<char> vis(n, 0);
        queue<int> q;
        vis[s] = 1;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto& e : g[u])
                if (e.cap > 0 && !vis[e.to]) {
                    vis[e.to] = 1;
                    q.push(e.to);
                }
        }
        return vis;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n, m;
        cin >> n >> m;
        vector<int> eu(m), ev(m);
        vector<vector<int>> adj(n + 1);
        for (int i = 0; i < m; i++) {
            cin >> eu[i] >> ev[i];
            adj[eu[i]].push_back(ev[i]);
        }
        vector<int> a(n + 1);
        for (int i = 1; i <= n; i++) cin >> a[i];

        // Node ids: s = 0, b_v = v (1..n), c_v = n + v (1..n), t = 2n + 1.
        const int S = 0, Tt = 2 * n + 1, N = 2 * n + 2;
        const long long INF = (long long)4e18;
        Dinic din(N);
        auto B = [&](int v) { return v; };
        auto C = [&](int v) { return n + v; };
        for (int v = 1; v <= n; v++) {
            din.addEdge(S, B(v), 1);     // weight +1 on b_v
            din.addEdge(C(v), Tt, 1);    // weight -1 on c_v
            din.addEdge(C(v), B(v), INF); // c_v => b_v
            if (a[v] == 0) din.addEdge(B(v), C(v), INF); // 0-vertex: b_v => c_v
        }
        for (int i = 0; i < m; i++) {
            din.addEdge(B(eu[i]), B(ev[i]), INF); // grp(u) <= grp(v): b_u => b_v
            din.addEdge(C(eu[i]), C(ev[i]), INF); //                   c_u => c_v
        }

        long long flow = din.maxflow(S, Tt);
        int L = n - (int)flow;

        vector<char> src = din.minCutSourceSide(S);
        vector<int> grp(n + 1);
        for (int v = 1; v <= n; v++)
            grp[v] = (src[B(v)] ? 1 : 0) + (src[C(v)] ? 1 : 0);

        // Build an optimal order: topological sort with group as primary key
        // (smaller group first). Valid because grp(u) <= grp(v) on every edge.
        vector<int> indeg(n + 1, 0);
        for (int i = 0; i < m; i++) indeg[ev[i]]++;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        for (int v = 1; v <= n; v++)
            if (indeg[v] == 0) pq.push({grp[v], v});
        vector<int> order;
        order.reserve(n);
        while (!pq.empty()) {
            auto [gg, v] = pq.top();
            pq.pop();
            order.push_back(v);
            for (int w : adj[v])
                if (--indeg[w] == 0) pq.push({grp[w], w});
        }

        cout << L << '\n';
        for (int i = 0; i < n; i++) cout << order[i] << " \n"[i + 1 == n];
        if (n == 0) cout << '\n';
    }
    return 0;
}
