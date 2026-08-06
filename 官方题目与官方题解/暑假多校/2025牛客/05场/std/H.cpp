#include<bits/stdc++.h>
using namespace std;
const int N = 1e2 + 7;
const int inf = 0x3f3f3f3f;
int m, s, t, n;
int a[N], d[N], b[N], c[N], sum[N];
int dp[N][N][N];

bool check(int x) {
    for (int i = 0; i <= t; i++) {
        for (int j = 0; j <= n && j <= i; j++) {
            for (int k = 0; k <= i; k++)dp[i][j][k] = -inf;
        }
    }
    dp[0][0][0] = 0;
    for (int j = 0; j <= n; j++) {
        for (int i = j; i <= t; i++) {
            for (int k = 0; k <= i; k++) {
                if (dp[i][j][k] >= s)return true;
                if (i + 1 <= t)dp[i + 1][j][k + 1] = max(dp[i + 1][j][k + 1], dp[i][j][k] + sum[j]);
                if (j + 1 <= n) {
                    int r = (a[j + 1] - 1) / sum[j] + 1;
                    if (i + r <= t)dp[i + r][j + 1][k + r] = max(dp[i + r][j + 1][k + r], dp[i][j][k]);
                    r = (a[j + 1] - c[j + 1] - 1) / sum[j] + 1;
                    if (!x)continue;
                    int nd = (b[j + 1] - 1) / x + 1;
                    if (i + r <= t && k + r - nd >= 0)
                        dp[i + r][j + 1][k + r - nd] = max(
                            dp[i + r][j + 1][k + r - nd], dp[i][j][k]);
                }
            }
        }
    }
    return false;
}

signed main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    cin >> m >> s >> t;
    cin >> n;
    for (int i = 1; i <= n; i++)cin >> a[i] >> d[i] >> b[i] >> c[i];
    sum[0] = m;
    for (int i = 1; i <= n; i++)sum[i] = sum[i - 1] + d[i];
    int L = 0, R = 10001;
    while (L < R) {
        int mid = (L + R) >> 1;
        if (check(mid))R = mid;
        else L = mid + 1;
    }
    if (R == 10001)cout << "-1\n";
    else cout << R << "\n";
    return 0;
}
