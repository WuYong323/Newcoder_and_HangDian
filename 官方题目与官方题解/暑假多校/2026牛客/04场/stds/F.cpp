#include <bits/stdc++.h>
using namespace std;
 
static const long long LIM = 1000000000000000000LL;
static const int INF = 1000000000;
 
struct FastInput {
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
 
    template <class T>
    bool readUnsigned(T &out) {
        char c = getChar();
        while (c && c <= ' ') c = getChar();
        if (!c) return false;
        T x = 0;
        while (c > ' ') {
            x = x * 10 + (c - '0');
            c = getChar();
        }
        out = x;
        return true;
    }
};
 
struct SegTreeMin {
    int n = 1;
    vector<int> t;
 
    void init(int m) {
        n = 1;
        while (n < m) n <<= 1;
        t.assign(2 * n, INF);
    }
 
    void reset() {
        fill(t.begin(), t.end(), INF);
    }
 
    inline void update(int p, int val) {
        if (val >= INF) return;
        int x = p + n - 1;
        if (val >= t[x]) return;
        t[x] = val;
        x >>= 1;
        while (x) {
            int nv = min(t[x << 1], t[x << 1 | 1]);
            if (nv == t[x]) break;
            t[x] = nv;
            x >>= 1;
        }
    }
 
    inline int query(int l, int r) const {
        if (l > r) return INF;
        int res = INF;
        l += n - 1;
        r += n - 1;
        while (l <= r) {
            if (l & 1) res = min(res, t[l++]);
            if (!(r & 1)) res = min(res, t[r--]);
            l >>= 1;
            r >>= 1;
        }
        return res;
    }
};
 
int main() {
    FastInput in;
    int n, q;
    if (!in.readUnsigned(n)) return 0;
    in.readUnsigned(q);
 
    vector<long long> a(n + 1), vals;
    vals.reserve(n);
    for (int i = 1; i <= n; ++i) {
        in.readUnsigned(a[i]);
        vals.push_back(a[i]);
    }
 
    vector<int> ql(q), qr(q), ans(q, 1);
    for (int i = 0; i < q; ++i) {
        in.readUnsigned(ql[i]);
        in.readUnsigned(qr[i]);
    }
 
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    int m = (int)vals.size();
 
    vector<int> pos(n + 1), leftId(n + 1), rightId(n + 1);
    for (int i = 1; i <= n; ++i) {
        long long lo = a[i] * 2LL;
        long long hi = a[i] * 3LL;
        pos[i] = (int)(lower_bound(vals.begin(), vals.end(), a[i]) - vals.begin()) + 1;
        leftId[i] = (int)(lower_bound(vals.begin(), vals.end(), lo) - vals.begin()) + 1;
        rightId[i] = (int)(upper_bound(vals.begin(), vals.end(), hi) - vals.begin());
    }
 
    int maxK = 0;
    for (__int128 x = 1; x <= (__int128)LIM; x <<= 1) ++maxK;
    maxK = min(maxK, n);
 
    vector<int> prev(n + 2), cur(n + 2), suf(n + 3);
    for (int i = 1; i <= n; ++i) prev[i] = i;
 
    SegTreeMin seg;
    seg.init(m);
 
    for (int len = 2; len <= maxK; ++len) {
        seg.reset();
        fill(cur.begin(), cur.end(), INF);
 
        for (int i = n; i >= 1; --i) {
            if (leftId[i] <= rightId[i]) {
                cur[i] = seg.query(leftId[i], rightId[i]);
            }
            seg.update(pos[i], prev[i]);
        }
 
        int best = INF;
        for (int i = n; i >= 1; --i) {
            if (cur[i] < best) best = cur[i];
            suf[i] = best;
        }
        if (best >= INF) break;
 
        for (int id = 0; id < q; ++id) {
            if (suf[ql[id]] <= qr[id]) ans[id] = len;
        }
 
        prev.swap(cur);
    }
 
    string out;
    out.reserve((size_t)q * 3);
    for (int i = 0; i < q; ++i) {
        out += to_string(ans[i]);
        out.push_back('\n');
    }
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
