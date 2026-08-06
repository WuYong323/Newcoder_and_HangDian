#include<bits/stdc++.h>
using namespace std;
const int inf = 0x3f3f3f3f;

void solve() {
    int n, k, s, t;
    cin >> n >> k >> s >> t;
    int d = 2 * min(k, n - k);
    int l = s, r = s;
    int ans = inf;
    for (int i = 0; i < 2; i++) {
        if (l % 2 == t % 2) {
            if (t >= l && t <= r)ans = min(ans, i);
            else if (d) {
                if (t < l)ans = min(ans, i + ((l - t - 1) / d + 1) * 2);
                if (t > r)ans = min(ans, i + ((t - r - 1) / d + 1) * 2);
            }
        }
        l = abs(l - k);
        r = n - abs(n - k - r);
    }
    if (ans == inf)cout << "-1\n";
    else cout << ans << "\n";
}

signed main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int T;
    cin >> T;
    while (T--)solve();
    return 0;
}