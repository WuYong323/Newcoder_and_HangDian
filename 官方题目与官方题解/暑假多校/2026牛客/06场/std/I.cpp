#include <bits/stdc++.h>

using i64 = long long;

constexpr int M = 998244353;

int add(int x, int y) {
    x += y;
    if (x >= M) x -= M;
    return x;
}

int mul(int x, int y) {
    return 1LL * x * y % M;
}

std::array<int, 4> f[62][2][2];

void solve() {
    i64 n, x;
    std::cin >> n >> x;
    for (int i = 0; i < 62; i++) {
        f[i][0][0].fill(-1);
        f[i][0][1].fill(-1);
        f[i][1][0].fill(-1);
        f[i][1][1].fill(-1);
    }
    auto dfs = [&](auto&& self, int i, int ish, int c) -> std::array<int, 4> {
        if (i == -1) return {(c == 0), 0, 0, 0};
        if (f[i][ish][c][0] != -1) return f[i][ish][c];
        std::array<int, 4> ans = {0};
        int hi = ish ? (n >> i & 1) : 1;
        for (int d = 0; d <= hi; d++) for (int nc = 0; nc < 2; nc++) {
            int s = d + (x >> i & 1) + nc;
            int q = s >> 1, r = s & 1;
            if (q == c) {
                auto nans = self(self, i - 1, ish & (d == hi), nc);
                ans[0] = add(ans[0], nans[0]);
                ans[1] = add(ans[1], nans[1]);
                ans[1] = add(ans[1], mul(nans[0], d));
                ans[2] = add(ans[2], nans[2]);
                ans[2] = add(ans[2], mul(nans[0], r));
                ans[3] = add(ans[3], nans[3]);
                ans[3] = add(ans[3], mul(nans[2], d));
                ans[3] = add(ans[3], mul(nans[1], r));
                ans[3] = add(ans[3], mul(nans[0], d & r));
            }
        }
        return f[i][ish][c] = ans;
    };
    std::cout << dfs(dfs, 61, 1, 0)[3] << "\n";
}

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);

    int t;
    std::cin >> t;
    while (t--) solve();
}