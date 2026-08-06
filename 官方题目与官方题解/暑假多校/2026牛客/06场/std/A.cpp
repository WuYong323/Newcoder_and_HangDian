#include <bits/stdc++.h>

constexpr int M = 998244353;

void solve() {
    int n;
    std::cin >> n;
    std::vector<int> p(n), q(n);
    for (int i = 0; i < n; i++) std::cin >> p[i], q[--p[i]] = i;

    std::vector<int> fa(n), sz(n, 1);
    std::iota(fa.begin(), fa.end(), 0);
    auto _find = [&](auto&& self, int u) -> int {
        return u == fa[u] ? u : fa[u] = self(self, fa[u]);
    };
    auto find = [&](int u) -> int {
        return _find(_find, u);
    };
    auto merge = [&](int u, int v) -> void {
        u = find(u);
        v = find(v);
        if (u == v || sz[u] == 0 || sz[v] == 0) return;
        if (sz[u] > sz[v]) std::swap(u, v);
        fa[u] = v;
        sz[v] += sz[u];
    };
    int ans = 1;
    for (int i = n - 1; i >= 0; i--) {
        int j = q[i];
        ans = 1LL * ans * sz[find(j)]-- % M;
        if (j > 0 && j + 1 < n) merge(j - 1, j + 1);
    }
    std::cout << ans << "\n";
}

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);

    int t;
    std::cin >> t;
    while (t--) solve();
}