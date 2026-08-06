#include <bits/stdc++.h>
using namespace std;

static constexpr int MOD = 998244353;

vector<int> build_h(int maxOmega) {
    vector<int> h(maxOmega + 1, 0);
    h[0] = 1;
    if (maxOmega >= 1) h[1] = 1;

    vector<int> parts;
    function<void(int, int, vector<long long>&)> dfs = [&](int rem, int last, vector<long long>& vals) {
        if (rem == 0) {
            if (parts.size() >= 2) {
                long long prod = 1;
                for (int x : parts) prod *= h[x];
                vals.push_back(prod);
            }
            return;
        }
        for (int x = min(last, rem); x >= 1; --x) {
            parts.push_back(x);
            dfs(rem - x, x, vals);
            parts.pop_back();
        }
    };

    for (int m = 2; m <= maxOmega; ++m) {
        vector<long long> vals;
        parts.clear();
        dfs(m, m, vals);
        vector<char> seen(10000, 0);
        seen[0] = 1;
        for (long long v : vals) {
            if (v < (long long)seen.size()) seen[(int)v] = 1;
        }
        int mex = 0;
        while (seen[mex]) ++mex;
        h[m] = mex;
    }
    return h;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, c;
    cin >> N >> c;

    int maxOmega = 0;
    for (int x = N; x > 1; x >>= 1) ++maxOmega;
    vector<int> h = build_h(maxOmega);

    vector<int> lp(N + 1, 0), primes;
    vector<unsigned char> omega(N + 1, 0);
    primes.reserve(N / 10);

    long long ans = 0;
    long long pw = 1;

    for (int i = 1; i <= N; ++i) {
        if (i >= 2) {
            if (lp[i] == 0) {
                lp[i] = i;
                primes.push_back(i);
            }
            omega[i] = (unsigned char)(omega[i / lp[i]] + 1);
            for (int p : primes) {
                long long v = 1LL * i * p;
                if (v > N || p > lp[i]) break;
                lp[(int)v] = p;
            }
        }

        pw = pw * c % MOD;
        ans += 1LL * h[omega[i]] * pw % MOD;
        if (ans >= MOD) ans -= MOD;
    }

    cout << ans % MOD << '\n';
    return 0;
}
