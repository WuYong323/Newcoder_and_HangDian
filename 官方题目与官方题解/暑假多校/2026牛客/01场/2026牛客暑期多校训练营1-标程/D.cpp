#include <bits/stdc++.h>
using namespace std;

static const long long MOD = 998244353;

long long mod_pow(long long a, long long e) {
    a %= MOD;
    long long r = 1;
    while (e > 0) {
        if (e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return r;
}

long long mod_inv(long long x) {
    return mod_pow(x, MOD - 2);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    cin >> m;

    vector<long long> s(m);
    for (int i = 0; i < m; i++) cin >> s[i];

    sort(s.begin(), s.end());

    const long long inv2 = (MOD + 1) / 2;

    long long ans = 0;
    long long pref = 1;   // product of inactive s_i modulo MOD
    long long prev = 0;   // previous distinct raw value

    for (int i = 0; i < m; ) {
        int j = i;
        while (j < m && s[j] == s[i]) j++;

        long long v = s[i];          // raw value, not reduced for comparisons
        long long r = m - i;         // number of active dimensions

        if (r > 1) {
            long long exp = r - 1;

            long long curPow = mod_pow(v % MOD, exp);
            long long prevPow = mod_pow(prev % MOD, exp);
            long long diff = (curPow - prevPow + MOD) % MOD;

            long long term = pref * diff % MOD;

            if (r & 1LL) {
                // r odd: multiply by 1/2
                term = term * inv2 % MOD;
            } else {
                // r even: multiply by r / (2(r - 1))
                long long denom = (2 * (r - 1)) % MOD;
                term = term * (r % MOD) % MOD * mod_inv(denom) % MOD;
            }

            ans += term;
            if (ans >= MOD) ans -= MOD;
        }

        // These equal values become inactive for later phases.
        long long cnt = j - i;
        pref = pref * mod_pow(v % MOD, cnt) % MOD;

        prev = v;
        i = j;
    }

    cout << ans % MOD << '\n';
    return 0;
}
