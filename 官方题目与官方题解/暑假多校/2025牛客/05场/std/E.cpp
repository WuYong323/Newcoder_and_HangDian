#include<bits/stdc++.h>
using namespace std;
const int N = 2e5 + 7;
typedef long long ll;
int n;
int a[N], sum[N];

signed main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    cin >> n;
    for (int i = 1; i <= n; i++)cin >> a[i];
    ll ans = 0;
    for (int j = 0; j <= 29; j++) {
        vector<vector<int> > cnt(2, vector<int>(2));
        for (int i = 1; i <= n; i++) {
            int x = (a[i] >> j & 1);
            ans += (1ll << j) * cnt[x ^ 1][sum[i] & 1];
            cnt[x][sum[i] & 1]++;
            sum[i] += x;
        }
    }
    cout << ans << "\n";
    return 0;
}
