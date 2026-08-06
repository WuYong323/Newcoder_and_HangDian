#include <bits/stdc++.h>
using namespace std;

static const long long MOD = 998244353LL;

long long norm_mod(long long x) {
    x %= MOD;
    if (x < 0) x += MOD;
    return x;
}

long long mod_pow(long long a, long long e) {
    a = norm_mod(a);
    long long r = 1;
    while (e > 0) {
        if (e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return r;
}

struct Solver {
    int K;
    vector<long long> fact, ifact;

    long long C(int n, int r) const {
        if (r < 0 || r > n) return 0;
        return fact[n] * ifact[r] % MOD * ifact[n - r] % MOD;
    }

    void init_comb(int k) {
        K = k;
        fact.assign(K + 1, 1);
        ifact.assign(K + 1, 1);

        for (int i = 1; i <= K; ++i) {
            fact[i] = fact[i - 1] * i % MOD;
        }

        ifact[K] = mod_pow(fact[K], MOD - 2);

        for (int i = K; i >= 1; --i) {
            ifact[i - 1] = ifact[i] * i % MOD;
        }
    }

    // p 的最后一个元素必须是 K。
    // 前面的 p[i] 是精确 prefix mex，最后一个 K 表示 0..K-1 全部出现。
    // H 表示额外的高值数量，它们不影响 mex < K。
    long long calc_prefix(const vector<int>& p, long long H) const {
        if (H < 0) return 0;

        int len = (int)p.size();

        if (K == 0) {
            return mod_pow(H, len);
        }

        int cur = 0;
        vector<int> t, r, ell;

        for (int i = 1; i <= len; ++i) {
            int v = p[i - 1];

            if (v < cur || v > K) return 0;

            if (v > cur) {
                t.push_back(i);
                r.push_back(v - cur - 1);
                cur = v;
            }
        }

        if (cur != K) return 0;

        int s = (int)t.size();

        ell.resize(s);

        int prev = 0;
        for (int j = 0; j < s; ++j) {
            ell[j] = t[j] - prev - 1;
            prev = t[j];
        }

        vector<long long> dp(1, 1);
        int maxD = 0;

        for (int j = s - 1; j >= 0; --j) {
            int rj = r[j];
            int future_pivots = s - j;

            vector<long long> pw(maxD + rj + 1);

            for (int d = 0; d <= maxD + rj; ++d) {
                long long base = norm_mod((H % MOD) + K - future_pivots - d);
                pw[d] = mod_pow(base, ell[j]);
            }

            vector<long long> ndp(maxD + rj + 1, 0);

            if (rj == 0) {
                for (int d = 0; d <= maxD; ++d) {
                    ndp[d] = dp[d] * pw[d] % MOD;
                }
            } else {
                for (int dnext = 0; dnext <= maxD; ++dnext) {
                    long long curWays = dp[dnext];

                    if (curWays == 0) continue;

                    for (int c = 0; c <= rj; ++c) {
                        int d = dnext + c;

                        long long ways = C(rj, c);
                        if (c & 1) ways = (MOD - ways) % MOD;

                        long long add = curWays * ways % MOD * pw[d] % MOD;

                        ndp[d] += add;
                        if (ndp[d] >= MOD) ndp[d] -= MOD;
                    }
                }
            }

            dp.swap(ndp);
            maxD += rj;
        }

        long long ans = 0;

        for (long long x : dp) {
            ans += x;
            if (ans >= MOD) ans -= MOD;
        }

        return ans;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    long long M;

    cin >> N >> M;

    vector<long long> B(max(0, N - 1));

    for (int i = 0; i < N - 1; ++i) {
        cin >> B[i];
    }

    if (M == 0) {
        cout << 0 << '\n';
        return 0;
    }

    if (N == 1) {
        cout << norm_mod(M) << '\n';
        return 0;
    }

    for (long long x : B) {
        if (x < 0 || x > M) {
            cout << 0 << '\n';
            return 0;
        }
    }

    long long Kll = 0;
    for (long long x : B) {
        Kll = max(Kll, x);
    }

    if (Kll == 0) {
        long long ans = mod_pow(M - 1, N);
        ans += (long long)(N % MOD) * mod_pow(M - 1, N - 1) % MOD;
        ans %= MOD;
        cout << ans << '\n';
        return 0;
    }

    // 峰值 K 需要 0..K-1 每个值在峰段左右各出现一次。
    if (Kll > M || Kll > N / 2) {
        cout << 0 << '\n';
        return 0;
    }

    int K = (int)Kll;

    int L0 = -1, R0 = -1;

    for (int i = 0; i < N - 1; ++i) {
        if (B[i] == Kll) {
            if (L0 == -1) L0 = i;
            R0 = i;
        }
    }

    // 检查 K 的峰段是否连续。
    for (int i = L0; i <= R0; ++i) {
        if (B[i] != Kll) {
            cout << 0 << '\n';
            return 0;
        }
    }

    // 左侧必须非降。
    for (int i = 0; i < L0; ++i) {
        if (B[i] > B[i + 1]) {
            cout << 0 << '\n';
            return 0;
        }
    }

    // 右侧必须非升。
    for (int i = R0; i + 1 < N - 1; ++i) {
        if (B[i] < B[i + 1]) {
            cout << 0 << '\n';
            return 0;
        }
    }

    int L = L0 + 1;
    int R = R0 + 1;

    int mid = R - L;

    vector<int> leftPath;
    leftPath.reserve(L);

    for (int i = 0; i < L0; ++i) {
        leftPath.push_back((int)B[i]);
    }

    leftPath.push_back(K);

    vector<int> rightPath;
    rightPath.reserve(N - R);

    for (int cut = N - 1; cut >= R + 1; --cut) {
        rightPath.push_back((int)B[cut - 1]);
    }

    rightPath.push_back(K);

    Solver sol;
    sol.init_comb(K);

    long long ans = 0;

    if (Kll == M) {
        long long FL = sol.calc_prefix(leftPath, 0);
        long long FR = sol.calc_prefix(rightPath, 0);

        ans = FL * FR % MOD * mod_pow(M, mid) % MOD;
    } else {
        long long h = M - K;

        long long FL = sol.calc_prefix(leftPath, h);
        long long FL0 = sol.calc_prefix(leftPath, h - 1);
        long long FL1 = norm_mod(FL - FL0);

        long long FR = sol.calc_prefix(rightPath, h);
        long long FR0 = sol.calc_prefix(rightPath, h - 1);
        long long FR1 = norm_mod(FR - FR0);

        long long noKMid = mod_pow(M - 1, mid);

        long long oneKMid = 0;
        if (mid > 0) {
            oneKMid = (long long)(mid % MOD) * mod_pow(M - 1, mid - 1) % MOD;
        }

        ans = (FL1 * FR0 + FL0 * FR1) % MOD * noKMid % MOD;
        ans = (ans + FL0 * FR0 % MOD * ((noKMid + oneKMid) % MOD)) % MOD;
    }

    cout << ans << '\n';
    return 0;
}
