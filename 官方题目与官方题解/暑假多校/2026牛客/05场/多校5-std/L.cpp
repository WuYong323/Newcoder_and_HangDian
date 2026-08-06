#include <bits/stdc++.h>
using namespace std;

class FastScanner {
    static const int BUFSIZE = 1 << 20;
    int idx = 0, size = 0;
    char buf[BUFSIZE];

    inline char getChar() {
        if (idx >= size) {
            size = (int)fread(buf, 1, BUFSIZE, stdin);
            idx = 0;
            if (size == 0) return 0;
        }
        return buf[idx++];
    }

public:
    template <class T>
    bool readInt(T &out) {
        char c;
        T sign = 1;
        T val = 0;
        c = getChar();
        while (c && c <= ' ') c = getChar();
        if (!c) return false;
        if (c == '-') {
            sign = -1;
            c = getChar();
        }
        for (; c >= '0' && c <= '9'; c = getChar()) {
            val = val * 10 + (c - '0');
        }
        out = val * sign;
        return true;
    }
};

static inline int rowL(int s, int n, int m) {
    return max(0, s - (m - 1));
}

static inline int rowR(int s, int n, int m) {
    return min(n - 1, s);
}

int main() {
    FastScanner fs;
    int TC;
    fs.readInt(TC);

    string output;
    output.reserve((size_t)TC * 24);

    while (TC--) {
        int n, m;
        fs.readInt(n);
        fs.readInt(m);

        const size_t cells = (size_t)n * (size_t)m;
        vector<int> a(cells);
        for (size_t i = 0; i < cells; ++i) {
            long long v;
            fs.readInt(v);
            a[i] = (int)v;
        }

        auto A = [&](int i, int j) -> long long {
            return (long long)a[(size_t)i * (size_t)m + (size_t)j];
        };
        auto X = [&](int i, int j, long long target) -> long long {
            return target - A(i, j);
        };

        long long ans = -1;

        if (n == 1 || m == 1) {
            bool same = true;
            for (size_t i = 1; i < cells; ++i) {
                if (a[i] != a[0]) {
                    same = false;
                    break;
                }
            }
            ans = same ? 0 : -1;
            output += to_string(ans);
            output += '\n';
            continue;
        }

        bool ok = true;

        if (A(0, 0) != A(n - 1, m - 1)) ok = false;

        long long k = A(0, 1) + A(1, 0) - 2LL * A(0, 0);
        if (k < 0) ok = false;

        long long target = A(0, 0) + k;

        if (ok) {
            for (int s = 0; s <= n + m - 2 && ok; ++s) {
                int L = rowL(s, n, m);
                int R = rowR(s, n, m);
                long long sum = 0;
                for (int i = L; i <= R; ++i) {
                    int j = s - i;
                    long long x = X(i, j, target);
                    if (x < 0) {
                        ok = false;
                        break;
                    }
                    sum += x;
                }
                if (ok && sum != k) ok = false;
            }
        }

        if (ok) {
            for (int s = 0; s <= n + m - 3 && ok; ++s) {
                int L1 = rowL(s, n, m), R1 = rowR(s, n, m);
                int L2 = rowL(s + 1, n, m), R2 = rowR(s + 1, n, m);
                int L = min(L1, L2), R = max(R1, R2);

                long long prefCur = 0;  // B_i
                long long prefNext = 0; // C_i

                for (int i = L; i <= R; ++i) {
                    long long prevCur = prefCur; // B_{i-1}

                    if (L1 <= i && i <= R1) {
                        int j = s - i;
                        prefCur += X(i, j, target);
                    }
                    if (L2 <= i && i <= R2) {
                        int j = s + 1 - i;
                        prefNext += X(i, j, target);
                    }

                    if (prefNext < prevCur || prefNext > prefCur) {
                        ok = false;
                        break;
                    }
                }
            }
        }

        ans = ok ? k : -1;
        output += to_string(ans);
        output += '\n';
    }

    fwrite(output.data(), 1, output.size(), stdout);
    return 0;
}
