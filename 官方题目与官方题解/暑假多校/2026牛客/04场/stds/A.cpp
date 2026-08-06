#include <bits/stdc++.h>
using namespace std;
 
using i128 = __int128_t;
 
const int N = 16;
 
i128 pw31[N + 1];
 
i128 abs128(i128 x) {
    return x < 0 ? -x : x;
}
 
i128 read_i128() {
    string s;
    cin >> s;
 
    bool neg = false;
    int p = 0;
 
    if (s[0] == '-') {
        neg = true;
        p = 1;
    }
 
    i128 x = 0;
    for (; p < (int)s.size(); p++) {
        x = x * 10 + (s[p] - '0');
    }
 
    return neg ? -x : x;
}
 
bool dfs(int k, i128 x, vector<int>& c) {
    if (k == 1) {
        if (-16 <= x && x <= 16) {
            c[0] = (int)x;
            return true;
        }
        return false;
    }
 
    i128 W = 16 * pw31[k - 2];
 
    vector<pair<i128, int>> cand;
 
    for (int v = -16; v <= 16; v++) {
        i128 rem = x - (i128)v * W;
 
        if (rem % 15 == 0) {
            i128 nxt = rem / 15;
 
            // 剩下 k - 1 位最多能表示的绝对值
            i128 maxPossible = 16 * pw31[k - 2];
 
            if (abs128(nxt) <= maxPossible) {
                cand.push_back({abs128(nxt), v});
            }
        }
    }
 
    sort(cand.begin(), cand.end());
 
    for (auto [_, v] : cand) {
        i128 nxt = (x - (i128)v * W) / 15;
        c[k - 1] = v;
 
        if (dfs(k - 1, nxt, c)) {
            return true;
        }
    }
 
    return false;
}
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    pw31[0] = 1;
    for (int i = 1; i <= N; i++) {
        pw31[i] = pw31[i - 1] * 31;
    }
 
    int T;
    cin >> T;
 
    while (T--) {
        i128 x = read_i128();
 
        vector<int> c(N, 0);
 
        bool ok = dfs(N, x, c);
        assert(ok);
 
        vector<vector<int>> a(N, vector<int>(N, 0));
 
        // 前 15 行
        for (int i = 0; i < N - 1; i++) {
            for (int j = 0; j <= i; j++) {
                if ((i + j) % 2 == 0) {
                    a[i][j] = 16;
                } else {
                    a[i][j] = -16;
                }
            }
 
            a[i][i + 1] = 15;
        }
 
        // 最后一行
        for (int j = 0; j < N; j++) {
            int sign = ((N + (j + 1)) % 2 == 0) ? 1 : -1;
            a[N - 1][j] = sign * c[j];
        }
 
        cout << N << '\n';
 
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (j) cout << ' ';
                cout << a[i][j];
            }
            cout << '\n';
        }
    }
 
    return 0;
}
