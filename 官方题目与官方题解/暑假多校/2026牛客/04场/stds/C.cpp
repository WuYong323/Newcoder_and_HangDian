#include <bits/stdc++.h>
using namespace std;
 
int main() {
    int n, m;
    cin >> n >> m;
 
    int B = 1 << m;
    vector<long long> cost((long long)m * B);
 
    for (int i = 0; i < n; i++) {
        vector<long long> d(m);
        for (int j = 0; j < m; j++)
            cin >> d[j];
        string s;
        cin >> s;
 
        int mask = 0;
        for (int j = 0; j < m; j++)
            if (s[j] == 'A')
                mask |= 1 << j;
 
        for (int j = 0; j < m; j++)
            cost[(long long)j * B + mask] += d[j];
    }
 
    for (int j = 0; j < m; j++) {
        long long *a = cost.data() + (long long)j * B;
        for (int bit = 0; bit < m; bit++) {
            int step = 1 << bit;
            int jump = step << 1;
            for (int start = 0; start < B; start += jump)
                for (int mask = start; mask < start + step; mask++)
                    a[mask] += a[mask + step];
        }
    }
 
    const long long INF = 4000000000000000000LL;
    vector<long long> dp(B, INF);
    dp[0] = 0;
 
    for (int mask = 0; mask < B; mask++) {
        long long cur = dp[mask];
        int rem = (B - 1) ^ mask;
        while (rem) {
            int bit = __builtin_ctz(rem);
            int nmask = mask | (1 << bit);
            long long value = cur + cost[(long long)bit * B + mask];
            if (value < dp[nmask])
                dp[nmask] = value;
            rem &= rem - 1;
        }
    }
 
    cout << dp[B - 1] << "\n";
}
