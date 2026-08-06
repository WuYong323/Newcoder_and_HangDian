#include <bits/stdc++.h>
using namespace std;

class FastScanner {
    static const int BUFSIZE = 1 << 20;
    char buf[BUFSIZE];
    int pos = 0, len = 0;

    char getChar() {
        if (pos >= len) {
            len = (int)fread(buf, 1, BUFSIZE, stdin);
            pos = 0;
            if (len == 0) return 0;
        }
        return buf[pos++];
    }

public:
    template <class T>
    bool readInt(T &out) {
        char c = getChar();
        if (!c) return false;
        while (c != '-' && (c < '0' || c > '9')) {
            c = getChar();
            if (!c) return false;
        }
        T sign = 1;
        if (c == '-') {
            sign = -1;
            c = getChar();
        }
        T val = 0;
        while (c >= '0' && c <= '9') {
            val = val * 10 + (c - '0');
            c = getChar();
        }
        out = val * sign;
        return true;
    }
};

class FastOutput {
    static const int BUFSIZE = 1 << 20;
    char buf[BUFSIZE];
    int pos = 0;

public:
    ~FastOutput() { flush(); }

    void flush() {
        if (pos) {
            fwrite(buf, 1, pos, stdout);
            pos = 0;
        }
    }

    void putChar(char c) {
        if (pos == BUFSIZE) flush();
        buf[pos++] = c;
    }

    void writeInt(int x, char after) {
        char s[16];
        int n = 0;
        if (x == 0) {
            s[n++] = '0';
        } else {
            while (x > 0) {
                s[n++] = char('0' + x % 10);
                x /= 10;
            }
        }
        while (n--) putChar(s[n]);
        putChar(after);
    }
};

struct SparseRMQ {
    int n = 0, K = 0;
    vector<int> stMax; // low[j] = min(a[j], a[j + 1])
    vector<int> stMin; // high[j] = max(a[j], a[j + 1])

    explicit SparseRMQ(const vector<int> &a) { build(a); }

    void build(const vector<int> &a) {
        n = (int)a.size();
        K = 32 - __builtin_clz((unsigned)n); // floor(log2(n)) + 1, n >= 1

        stMax.assign((size_t)K * n, 0);
        stMin.assign((size_t)K * n, 0);

        for (int i = 0; i < n; ++i) {
            int j = (i + 1 == n ? 0 : i + 1);
            stMax[i] = min(a[i], a[j]);
            stMin[i] = max(a[i], a[j]);
        }

        for (int level = 1; level < K; ++level) {
            int len = 1 << level;
            int half = len >> 1;
            int limit = n - len + 1;
            size_t cur = (size_t)level * n;
            size_t prev = (size_t)(level - 1) * n;
            for (int i = 0; i < limit; ++i) {
                stMax[cur + i] = max(stMax[prev + i], stMax[prev + i + half]);
                stMin[cur + i] = min(stMin[prev + i], stMin[prev + i + half]);
            }
        }
    }

    int queryMaxLinear(int l, int r) const {
        int len = r - l + 1;
        int level = 31 - __builtin_clz((unsigned)len);
        int seg = 1 << level;
        size_t off = (size_t)level * n;
        return max(stMax[off + l], stMax[off + r - seg + 1]);
    }

    int queryMinLinear(int l, int r) const {
        int len = r - l + 1;
        int level = 31 - __builtin_clz((unsigned)len);
        int seg = 1 << level;
        size_t off = (size_t)level * n;
        return min(stMin[off + l], stMin[off + r - seg + 1]);
    }

    pair<int, int> queryCircular(int start, int len) const {
        if (len >= n) {
            return {queryMaxLinear(0, n - 1), queryMinLinear(0, n - 1)};
        }

        int end = start + len - 1;
        if (end < n) {
            return {queryMaxLinear(start, end), queryMinLinear(start, end)};
        }

        int end2 = end - n;
        int mx = max(queryMaxLinear(start, n - 1), queryMaxLinear(0, end2));
        int mn = min(queryMinLinear(start, n - 1), queryMinLinear(0, end2));
        return {mx, mn};
    }
};

int main() {
    FastScanner fs;

    int n;
    long long k;
    if (!fs.readInt(n)) return 0;
    fs.readInt(k);

    vector<int> a(n);
    for (int i = 0; i < n; ++i) fs.readInt(a[i]);

    FastOutput out;

    if (k == 0 || n == 1) {
        for (int i = 0; i < n; ++i) out.writeInt(a[i], i + 1 == n ? '\n' : ' ');
        return 0;
    }

    if (n == 2) {
        if (k & 1LL) swap(a[0], a[1]);
        out.writeInt(a[0], ' ');
        out.writeInt(a[1], '\n');
        return 0;
    }

    SparseRMQ rmq(a);

    int D = (int)min<long long>(k, (n - 1) / 2);

    auto queryLR = [&](int i, int d) -> pair<int, int> {
        // Edges touching cells within distance d from i:
        // [i - d - 1, ..., i + d] on the edge cycle.
        int start = i - d - 1;
        if (start < 0) start += n; // d <= (n - 1) / 2, so one addition is enough.

        int len = 2 * d + 2;
        if (len > n) len = n; // for odd n and maximal D, the raw interval has n + 1 edges.

        return rmq.queryCircular(start, len);
    };

    vector<int> ans(n);

    for (int i = 0; i < n; ++i) {
        // L(d) is nondecreasing, R(d) is nonincreasing.
        // Find the last d with L(d) <= R(d).
        int lo = -1, hi = D + 1;
        while (hi - lo > 1) {
            int mid = (lo + hi) >> 1;
            auto [L, R] = queryLR(i, mid);
            if (L <= R) lo = mid;
            else hi = mid;
        }

        int best = -1;

        // Source part: max_d min(L(d), R(d)).
        if (lo >= 0) {
            auto [L, R] = queryLR(i, lo);
            best = max(best, L);
        }
        if (lo + 1 <= D) {
            auto [L, R] = queryLR(i, lo + 1);
            best = max(best, R);
        }

        // Pure alternating part: no equal adjacent pair in radius D.
        auto [LD, RD] = queryLR(i, D);
        int alt = -1;
        if ((k & 1LL) == 0) {
            int cand = min(a[i], RD);
            if (cand > LD) alt = cand;
        } else {
            int cand = RD;
            if (cand > max(a[i], LD)) alt = cand;
        }

        ans[i] = max(best, alt);
    }

    for (int i = 0; i < n; ++i) {
        out.writeInt(ans[i], i + 1 == n ? '\n' : ' ');
    }

    return 0;
}
