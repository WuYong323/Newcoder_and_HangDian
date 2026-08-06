#include <bits/stdc++.h>
using namespace std;

using LL = long long;
using uLL = unsigned long long;
using Poly = vector<int>;

static const int Mod = 998244353;
static const int G = 3;

vector<uLL> Grt, iGrt;

inline int qpow(int x, long long y, int z = 1) {
    for (; y; y >>= 1, x = (LL)x * x % Mod) {
        if (y & 1) z = (LL)z * x % Mod;
    }
    return z;
}

inline uLL trans(const uLL &x) {
    constexpr uLL A = -(uLL)Mod / Mod + 1;
    constexpr uLL Q = (((__uint128_t)(-(uLL)Mod % Mod) << 64) + Mod - 1) / Mod;
    return x * A + (uLL)((__uint128_t)x * Q >> 64) + 1;
}

inline uLL mul_root(const uLL &x, const uLL &y) {
    return x * y * (__uint128_t)Mod >> 64;
}

inline int add_mod(int x, const int &y) {
    return ((x += y) - Mod) >= 0 ? x - Mod : x;
}

inline int sub_mod(int x, const int &y) {
    return (x -= y) < 0 ? x + Mod : x;
}

inline void extend_roots(const int &n) {
    if (Grt.empty()) {
        Grt.emplace_back(trans(1));
        iGrt.emplace_back(trans(1));
    }
    if ((int)Grt.size() < n) {
        int L = (int)Grt.size();
        Grt.resize(n);
        iGrt.resize(n);
        for (; L < n; L <<= 1) {
            const int w = qpow(G, Mod / (L * 4));
            const int iw = qpow(w, Mod - 2);
            for (int i = 0; i < L; ++i) {
                Grt[i + L] = trans(mul_root(Grt[i], w));
                iGrt[i + L] = trans(mul_root(iGrt[i], iw));
            }
        }
    }
}

template<int A, int B, int C = 0, class fun>
inline void Butterrep(int i, int j, int k, fun F) {
    if (A != C) {
        F(i, j + C / B, k + C * 2 - C % B);
        Butterrep<A, B, C + (C < A)>(i, j, k, F);
    }
}

template<int i, class fun>
inline void Butter(int n, fun F) {
    if (n > 32) {
        for (int j = 0; 2 * i * j < n; j += 32 / i) {
            Butterrep<32, i>(i, j, 2 * i * j, F);
        }
    } else if (i < n) {
        for (int j = 0; 2 * i * j < n; ++j) {
            for (int k = 0; k < i; ++k) F(i, j, k + 2 * i * j);
        }
    }
}

template<class T>
inline void DFT_iter(T P, int n) {
    extend_roots(n);
    const auto F = [&](int x, int y, int z) {
        const int a = P[z];
        const int b = (int)mul_root(P[z + x], Grt[y]);
        P[z] = add_mod(a, b);
        P[z + x] = sub_mod(a, b);
    };
    for (int i = n >> 1; i > 16; i >>= 1) {
        for (int j = 0; 2 * i * j < n; ++j) {
            for (int k = 0; k < i; k += 32) Butterrep<32, 32>(i, j, k + 2 * i * j, F);
        }
    }
    Butter<16>(n, F);
    Butter<8>(n, F);
    Butter<4>(n, F);
    Butter<2>(n, F);
    Butter<1>(n, F);
}

template<class T>
inline void IDFT_iter(T P, int n) {
    const uLL ni = trans(qpow(n, Mod - 2));
    for (int i = 0; i < n; ++i) P[i] = (int)mul_root(P[i], ni);
    extend_roots(n);
    const auto F = [&](int x, int y, int z) {
        const int a = P[z], b = P[z + x];
        P[z] = add_mod(a, b);
        P[z + x] = (int)mul_root((uLL)(a - b + Mod), iGrt[y]);
    };
    Butter<1>(n, F);
    Butter<2>(n, F);
    Butter<4>(n, F);
    Butter<8>(n, F);
    Butter<16>(n, F);
    for (int i = 32; i < n; i <<= 1) {
        for (int j = 0; 2 * i * j < n; ++j) {
            for (int k = 0; k < i; k += 32) Butterrep<32, 32>(i, j, k + 2 * i * j, F);
        }
    }
}

inline void DFT(Poly &P) { DFT_iter(P.begin(), (int)P.size()); }
inline void IDFT(Poly &P) { IDFT_iter(P.begin(), (int)P.size()); }

inline Poly mul_gf2(const Poly &A, const Poly &B, int lim) {
    Poly res(lim, 0);
    if (lim <= 0) return res;

    int n = min((int)A.size(), lim);
    int m = min((int)B.size(), lim);
    while (n > 0 && A[n - 1] == 0) --n;
    while (m > 0 && B[m - 1] == 0) --m;
    if (n == 0 || m == 0) return res;

    vector<int> pa, pb;
    pa.reserve(n); pb.reserve(m);
    for (int i = 0; i < n; ++i) if (A[i]) pa.push_back(i);
    for (int i = 0; i < m; ++i) if (B[i]) pb.push_back(i);

    const long long sparseWork = 1LL * pa.size() * pb.size();
    if (sparseWork <= 700000LL) {
        if (pa.size() > pb.size()) swap(pa, pb);
        for (int i : pa) {
            const int upto = lim - i;
            for (int j : pb) {
                if (j >= upto) break;
                res[i + j] ^= 1;
            }
        }
        return res;
    }

    if (1LL * n * m <= 350000LL) {
        if (n <= m) {
            for (int i = 0; i < n; ++i) if (A[i]) {
                const int upto = min(m, lim - i);
                for (int j = 0; j < upto; ++j) if (B[j]) res[i + j] ^= 1;
            }
        } else {
            for (int j = 0; j < m; ++j) if (B[j]) {
                const int upto = min(n, lim - j);
                for (int i = 0; i < upto; ++i) if (A[i]) res[i + j] ^= 1;
            }
        }
        return res;
    }

    int needFull = n + m - 1;
    int need = min(lim, needFull);
    int sz = 1;
    while (sz < needFull) sz <<= 1;

    Poly fa(sz, 0), fb(sz, 0);
    for (int i = 0; i < n; ++i) fa[i] = A[i];
    for (int i = 0; i < m; ++i) fb[i] = B[i];

    DFT(fa); DFT(fb);
    for (int i = 0; i < sz; ++i) fa[i] = (LL)fa[i] * fb[i] % Mod;
    IDFT(fa);

    for (int i = 0; i < need; ++i) res[i] = fa[i] & 1;
    return res;
}

inline Poly square_gf2(const Poly &a, int lim) {
    Poly r(lim, 0);
    for (int i = 0; i < (int)a.size() && 2 * i < lim; ++i) {
        if (a[i]) r[2 * i] = 1;
    }
    return r;
}

inline Poly inverse_gf2(const Poly &f, int lim) {
    Poly g(1, 1);
    while ((int)g.size() < lim) {
        int nxt = min(lim, (int)g.size() << 1);
        Poly g2 = square_gf2(g, nxt);
        g = mul_gf2(f, g2, nxt);
    }
    g.resize(lim);
    return g;
}

inline Poly pow_gf2(Poly base, int exp, int lim) {
    Poly res(lim, 0);
    if (lim > 0) res[0] = 1;
    while (exp > 0) {
        if (exp & 1) res = mul_gf2(res, base, lim);
        exp >>= 1;
        if (exp) base = square_gf2(base, lim);
    }
    return res;
}

inline Poly lambda0(const Poly &a, int len) {
    Poly r(len, 0);
    for (int i = 0; i < len; ++i) {
        int p = i << 1;
        if (p < (int)a.size()) r[i] = a[p];
    }
    return r;
}

inline Poly lambda1(const Poly &a, int len) {
    Poly r(len, 0);
    for (int i = 0; i < len; ++i) {
        int p = (i << 1) | 1;
        if (p < (int)a.size()) r[i] = a[p];
    }
    return r;
}

inline Poly mul_by_lift2(const Poly &P, const Poly &E, int lim) {
    const int evenLen = (lim + 1) >> 1;
    const int oddLen = lim >> 1;
    Poly Pe = lambda0(P, evenLen);
    Poly Po = lambda1(P, oddLen);
    Poly Re = mul_gf2(Pe, E, evenLen);
    Poly Ro = mul_gf2(Po, E, oddLen);
    Poly R(lim, 0);
    for (int i = 0; i < evenLen; ++i) R[2 * i] = Re[i];
    for (int i = 0; i < oddLen; ++i) R[2 * i + 1] = Ro[i];
    return R;
}

// Returns {Q_{H-1}, Q_H} modulo x^lim.
pair<Poly, Poly> compute_Q(unsigned long long H, int lim) {
    Poly q0(lim, 0), q1(lim, 0);
    if (lim > 0) q0[0] = q1[0] = 1;
    if (lim > 1) q1[1] = 1;
    if (H == 1) return {q0, q1};

    Poly qm2 = q0, qm1 = q1;
    unsigned long long cur = 1;
    while (cur < H) {
        unsigned long long nh = cur + 1;
        unsigned long long shift = (nh - 1 >= 63 ? (unsigned long long)lim : (1ULL << (nh - 1)));
        if (shift >= (unsigned long long)lim) return {qm1, qm1};
        Poly q = qm1;
        int sh = (int)shift;
        for (int i = 0; i + sh < lim; ++i) if (qm2[i]) q[i + sh] ^= 1;
        qm2 = move(qm1);
        qm1 = move(q);
        cur = nh;
    }
    return {qm2, qm1};
}

// V(x)=W(x^2). Return s_i=[x^i] A(x)V(x)^i, 0<=i<len.
Poly solve_cartier_evenV(Poly A, const Poly &W, int len) {
    A.resize(len);
    if (len == 1) return Poly{A[0]};

    int evenLen = (len + 1) >> 1;
    int oddLen = len >> 1;

    Poly Aeven = lambda0(A, evenLen);
    Poly Aodd = lambda1(A, oddLen);
    Aodd = mul_gf2(Aodd, W, oddLen);

    Poly Seven = solve_cartier_evenV(move(Aeven), W, evenLen);
    Poly Sodd = solve_cartier_evenV(move(Aodd), W, oddLen);

    Poly ans(len, 0);
    for (int i = 0; i < evenLen; ++i) ans[2 * i] = Seven[i];
    for (int i = 0; i < oddLen; ++i) ans[2 * i + 1] = Sodd[i];
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        unsigned long long H;
        int N, K;
        cin >> H >> N >> K;

        string out;
        out.reserve(K);

        if (H == 1) {
            for (int k = 1; k <= K; ++k) out.push_back(k == N + 1 ? '1' : '0');
            cout << out << '\n';
            continue;
        }

        const int n = N + 1;
        const int half = (n + 1) >> 1;

        // D = Q_{H-1} mod x^n.
        Poly D = compute_Q(H, n).first;
        Poly invD = inverse_gf2(D, n);
        Poly invD2 = square_gf2(invD, n);
        Poly Dinv3 = mul_gf2(invD, invD2, n);

        // B = B_{H-1} = Q_{H-2}^2 / Q_{H-1}, only to degree < ceil(n/2).
        Poly Qprev = compute_Q(H - 1, half).first;
        Poly invD_half(invD.begin(), invD.begin() + half);
        Poly B = mul_gf2(square_gf2(Qprev, half), invD_half, half);

        // V = U^{-1} = B^{-1}(x^2), so only W = B^{-1} is needed by Cartier.
        Poly W = inverse_gf2(B, half);

        // A = D^{-3} * U^N = D^{-3} * (B^N)(x^2).
        Poly Bpow = pow_gf2(B, N, half);
        Poly A = mul_by_lift2(Dinv3, Bpow, n);

        Poly P = solve_cartier_evenV(move(A), W, n);

        for (int k = 1; k <= K; ++k) {
            out.push_back(char('0' + P[N - k + 1]));
        }
        cout << out << '\n';
    }
    return 0;
}
