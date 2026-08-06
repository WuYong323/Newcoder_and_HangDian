#include <bits/stdc++.h>
using namespace std;

static const int MOD = 998244353;

inline int addmod(int a, int b){ a += b; if(a >= MOD) a -= MOD; return a; }
inline int submod(int a, int b){ a -= b; if(a < 0) a += MOD; return a; }
inline int mulmod(long long a, long long b){ return int(a * b % MOD); }

int mod_pow(long long a, long long e){
    long long r = 1 % MOD;
    a %= MOD;
    while(e){
        if(e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return int(r);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int A,B,C,D;
    long long p1,p2,p3,p4;
    if(!(cin >> A >> B >> C >> D >> p1 >> p2 >> p3 >> p4)) return 0;

    int N = max(max(A,D), max(B,C));
    vector<int> spf(N + 1), phi(N + 1), primes;
    vector<signed char> mu(N + 1);
    primes.reserve(N / 10 + 10);
    phi[1] = 1;
    mu[1] = 1;
    for(int i=2;i<=N;i++){
        if(!spf[i]){
            spf[i] = i;
            primes.push_back(i);
            phi[i] = i - 1;
            mu[i] = -1;
        }
        for(int p: primes){
            long long v = 1LL * i * p;
            if(v > N || p > spf[i]) break;
            spf[v] = p;
            if(p == spf[i]){
                phi[v] = phi[i] * p;
                mu[v] = 0;
                break;
            }else{
                phi[v] = phi[i] * (p - 1);
                mu[v] = -mu[i];
            }
        }
    }
    if(N >= 1 && spf[1] == 0) spf[1] = 1;

    auto build_pow = [&](long long e, int lim){
        vector<int> pw(lim + 1), pp(N + 1);
        pw[1] = 1;
        for(int p: primes){
            if(p > lim) break;
            pp[p] = mod_pow(p, e);
        }
        for(int x=2; x<=lim; x++){
            int p = spf[x];
            pw[x] = mulmod(pw[x / p], pp[p]);
        }
        return pw;
    };

    vector<int> pow1 = build_pow(p1, A);
    vector<int> pow4 = build_pow(p4, D);
    vector<int> pref1(A + 1), pref4(D + 1);
    for(int i=1;i<=A;i++) pref1[i] = addmod(pref1[i-1], pow1[i]);
    for(int i=1;i<=D;i++) pref4[i] = addmod(pref4[i-1], pow4[i]);
    vector<int> pow2 = build_pow(p2, B);
    vector<int> pow3 = build_pow(p3, C);

    auto F_left = [&](int q)->int{
        if(q > A) return 0;
        return mulmod(pow1[q], pref1[A / q]);
    };
    auto F_right = [&](int q)->int{
        if(q > D) return 0;
        return mulmod(pow4[q], pref4[D / q]);
    };

    int maxBC = max(B, C);
    vector<int> need(maxBC / 1 + 1), f(maxBC + 1), h(maxBC + 1), part(maxBC + 1), seen(maxBC + 1, 0);

    auto get_part = [&](int g, int p)->int{
        if(seen[p] == g) return part[p];
        seen[p] = g;
        int t = 1;
        int x = g;
        while(x % p == 0){ t *= p; x /= p; }
        part[p] = t;
        return t;
    };

    auto build_need = [&](int g, int m){
        need[1] = 1;
        for(int d=2; d<=m; d++){
            int p = spf[d];
            int u = d / p;
            int v = need[u] * p;
            if(u % p != 0) v *= get_part(g, p);
            need[d] = v;
        }
    };

    auto divisor_zeta = [&](int *a, int m){
        for(int p: primes){
            if(p > m) break;
            int lim = m / p;
            for(int i=1; i<=lim; i++){
                int &x = a[i * p];
                x += a[i];
                if(x >= MOD) x -= MOD;
            }
        }
    };
    auto multiple_zeta = [&](int *a, int m){
        for(int p: primes){
            if(p > m) break;
            for(int i=m / p; i>=1; --i){
                int &x = a[i];
                x += a[i * p];
                if(x >= MOD) x -= MOD;
            }
        }
    };

    long long ans = 0;
    int G = min(B, C);
    for(int g=1; g<=G; g++){
        int MB = B / g;
        int MC = C / g;
        int M = max(MB, MC);
        build_need(g, M);

        for(int d=1; d<=MB; d++){
            f[d] = mulmod(phi[d], F_left(need[d]));
        }
        divisor_zeta(f.data(), MB);
        for(int x=1; x<=MB; x++){
            f[x] = mulmod(f[x], pow2[g * x]);
        }
        multiple_zeta(f.data(), MB);

        for(int d=1; d<=MC; d++){
            h[d] = mulmod(phi[d], F_right(need[d]));
        }
        divisor_zeta(h.data(), MC);
        for(int y=1; y<=MC; y++){
            h[y] = mulmod(h[y], pow3[g * y]);
        }
        multiple_zeta(h.data(), MC);

        long long cur = 0;
        int T = min(MB, MC);
        for(int t=1; t<=T; t++){
            int mt = mu[t];
            if(mt == 0) continue;
            long long prod = 1LL * f[t] * h[t] % MOD;
            if(mt == 1) cur += prod;
            else cur -= prod;
            if(cur >= (long long)MOD * MOD || cur <= -(long long)MOD * MOD) cur %= MOD;
        }
        cur %= MOD;
        if(cur < 0) cur += MOD;
        ans += (long long)(g % MOD) * cur % MOD;
        if(ans >= (long long)MOD * MOD) ans %= MOD;
    }
    ans %= MOD;
    cout << ans << '\n';
    return 0;
}
