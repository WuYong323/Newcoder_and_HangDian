#include <bits/stdc++.h>

using u32 = unsigned;

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);

    std::string s;
    std::cin >> s;
    int n = s.size();
    std::vector<int> z(n);
    for (int i = 1, l = 0, r = 0; i < n; i++) {
        if (i <= r && z[i - l] < r - i + 1) {
            z[i] = z[i - l];
        } else {
            z[i] = std::max(0, r - i + 1);
            while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        }
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    for (int i = 1; i < n; i++) if (i + z[i] == n) return std::cout << 0, 0;
    std::vector<u32> g(26);
    for (int i = 1; i < n; i++) g[s[z[i]] - 'a'] |= 1 << (s[i + z[i]] - 'a');
    std::vector<u32> f(1 << 26);
    f[0] = 1;
    for (int u = 0; u < 1 << 26; u++) for (int i = 0; i < 26; i++) if (~u >> i & 1) {
        if (!(u & g[i])) f[u | (1 << i)] += f[u];
    }
    std::cout << f.back();
}