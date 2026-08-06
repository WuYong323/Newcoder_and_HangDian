#include <bits/stdc++.h>
using namespace std;

const long long MOD = 998244353;

long long modPow(long long a, long long e) {
    long long result = 1;
    while (e > 0) {
        if (e & 1) {
            result = result * a % MOD;
        }
        a = a * a % MOD;
        e >>= 1;
    }
    return result;
}

struct Query {
    int n;
    int m;
    int c;
    int a;
    int b;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    vector<Query> queries(T);
    int maxM = 0;
    for (auto &query : queries) {
        cin >> query.n >> query.m >> query.c >> query.a >> query.b;
        maxM = max(maxM, query.m);
    }

    vector<long long> fact(maxM + 1), invFact(maxM + 1);
    fact[0] = 1;
    for (int i = 1; i <= maxM; ++i) {
        fact[i] = fact[i - 1] * i % MOD;
    }

    invFact[maxM] = modPow(fact[maxM], MOD - 2);
    for (int i = maxM; i >= 1; --i) {
        invFact[i - 1] = invFact[i] * i % MOD;
    }

    auto comb = [&](int n, int k) -> long long {
        if (k < 0 || k > n) {
            return 0;
        }
        return fact[n] * invFact[k] % MOD * invFact[n - k] % MOD;
    };

    for (const Query &query : queries) {
        if (query.m < query.n) {
            cout << 0 << '\n';
            continue;
        }

        int extra = query.m - query.n;
        if (extra % query.c != 0) {
            cout << 0 << '\n';
            continue;
        }

        int wins = extra / query.c;

        long long invB = modPow(query.b, MOD - 2);
        long long p = query.a * invB % MOD;
        long long q = (query.b - query.a) * invB % MOD;

        long long answer = query.n;
        answer = answer * modPow(query.m, MOD - 2) % MOD;
        answer = answer * comb(query.m, wins) % MOD;
        answer = answer * modPow(p, wins) % MOD;
        answer = answer * modPow(q, query.m - wins) % MOD;

        cout << answer << '\n';
    }

    return 0;
}
