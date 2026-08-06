#include <bits/stdc++.h>
using namespace std;
 
namespace extreme_fold {
 
static constexpr int INFV = 2000000010;
static constexpr int NINFV = -1;
static constexpr int LOW = 0;
static constexpr int HIGH = 1;
 
struct Pool {
    struct Node {
        int l, r;
        uint32_t pri;
        int ref, sz;
        int val;
        int firstVal, lastVal;
        int minPrevLow, maxPrevHigh;
        int minNextLow, maxNextHigh;
        unsigned char type, firstType, lastType, pad;
    };
 
    vector<Node> t;
    int freeHead = 0;
    uint32_t rng = 123456789u;
 
    void reset(size_t reserve_nodes = 0) {
        t.clear();
        if (reserve_nodes) t.reserve(reserve_nodes);
        t.push_back(Node{});
        freeHead = 0;
        rng = 123456789u;
    }
 
    uint32_t rng32() {
        uint32_t x = rng;
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        rng = x;
        return x;
    }
 
    int alloc() {
        int id;
        if (freeHead) {
            id = freeHead;
            freeHead = t[id].l;
            t[id] = Node{};
        } else {
            id = (int)t.size();
            t.push_back(Node{});
        }
        return id;
    }
 
    inline int sz(int x) const { return x ? t[x].sz : 0; }
 
    inline void retain(int x) {
        if (x) ++t[x].ref;
    }
 
    void release(int x) {
        if (!x) return;
        Node &u = t[x];
        if (--u.ref == 0) {
            int L = u.l, R = u.r;
            release(L);
            release(R);
            u.l = freeHead;
            freeHead = x;
        }
    }
 
    inline int hold(int x) {
        retain(x);
        return x;
    }
 
    struct Info {
        int sz;
        int firstVal, lastVal;
        unsigned char firstType, lastType;
        int minPrevLow, maxPrevHigh;
        int minNextLow, maxNextHigh;
    };
 
    inline Info emptyInfo() const {
        return {0, 0, 0, LOW, LOW, INFV, NINFV, INFV, NINFV};
    }
 
    inline Info singleInfo(int v, int tp) const {
        return {1, v, v, (unsigned char)tp, (unsigned char)tp, INFV, NINFV, INFV, NINFV};
    }
 
    inline Info info(int x) const {
        if (!x) return emptyInfo();
        const Node &u = t[x];
        return {u.sz, u.firstVal, u.lastVal, u.firstType, u.lastType,
                u.minPrevLow, u.maxPrevHigh, u.minNextLow, u.maxNextHigh};
    }
 
    inline Info combineInfo(const Info &A, const Info &B) const {
        if (A.sz == 0) return B;
        if (B.sz == 0) return A;
 
        Info C;
        C.sz = A.sz + B.sz;
        C.firstVal = A.firstVal;
        C.firstType = A.firstType;
        C.lastVal = B.lastVal;
        C.lastType = B.lastType;
        C.minPrevLow = min(A.minPrevLow, B.minPrevLow);
        C.maxPrevHigh = max(A.maxPrevHigh, B.maxPrevHigh);
        C.minNextLow = min(A.minNextLow, B.minNextLow);
        C.maxNextHigh = max(A.maxNextHigh, B.maxNextHigh);
 
        if (B.firstType == LOW) C.minPrevLow = min(C.minPrevLow, A.lastVal);
        else C.maxPrevHigh = max(C.maxPrevHigh, A.lastVal);
 
        if (A.lastType == LOW) C.minNextLow = min(C.minNextLow, B.firstVal);
        else C.maxNextHigh = max(C.maxNextHigh, B.firstVal);
 
        return C;
    }
 
    inline void applyInfo(int x, const Info &I) {
        Node &u = t[x];
        u.sz = I.sz;
        u.firstVal = I.firstVal;
        u.lastVal = I.lastVal;
        u.firstType = I.firstType;
        u.lastType = I.lastType;
        u.minPrevLow = I.minPrevLow;
        u.maxPrevHigh = I.maxPrevHigh;
        u.minNextLow = I.minNextLow;
        u.maxNextHigh = I.maxNextHigh;
    }
 
    inline void pull(int x) {
        const Node &u = t[x];
        Info res = combineInfo(
            combineInfo(info(u.l), singleInfo(u.val, u.type)),
            info(u.r)
        );
        applyInfo(x, res);
    }
 
    int newLeaf(int v, int tp) {
        int x = alloc();
        Node &u = t[x];
        u.l = u.r = 0;
        u.pri = rng32();
        u.ref = 1;
        u.val = v;
        u.type = (unsigned char)tp;
        pull(x);
        return x;
    }
 
    inline int cloneChangeR(int a) {
        int x = alloc();
        t[x] = t[a];
        t[x].ref = 1;
        retain(t[x].l);
        t[x].r = 0;
        return x;
    }
 
    inline int cloneChangeL(int a) {
        int x = alloc();
        t[x] = t[a];
        t[x].ref = 1;
        retain(t[x].r);
        t[x].l = 0;
        return x;
    }
 
    int mergeTreap(int a, int b) {
        if (!a) return hold(b);
        if (!b) return hold(a);
 
        if (t[a].pri < t[b].pri) {
            int x = cloneChangeR(a);
            t[x].r = mergeTreap(t[a].r, b);
            pull(x);
            return x;
        } else {
            int x = cloneChangeL(b);
            t[x].l = mergeTreap(a, t[b].l);
            pull(x);
            return x;
        }
    }
 
    pair<int, int> splitTreap(int x, int k) {
        if (!x) return {0, 0};
        if (k <= 0) return {0, hold(x)};
        if (k >= sz(x)) return {hold(x), 0};
 
        int lsz = sz(t[x].l);
        if (k <= lsz) {
            int y = cloneChangeL(x);
            auto pr = splitTreap(t[x].l, k);
            t[y].l = pr.second;
            pull(y);
            return {pr.first, y};
        } else {
            int y = cloneChangeR(x);
            auto pr = splitTreap(t[x].r, k - lsz - 1);
            t[y].r = pr.first;
            pull(y);
            return {y, pr.second};
        }
    }
 
    int typeAt(int x, int k) const {
        while (x) {
            int lsz = sz(t[x].l);
            if (k < lsz) x = t[x].l;
            else if (k == lsz) return t[x].type;
            else {
                k -= lsz + 1;
                x = t[x].r;
            }
        }
        return LOW;
    }
 
    int findLastLowPrevLE(int x, int v, int add = 0) const {
        if (!x || t[x].minPrevLow > v) return -1;
        int lsz = sz(t[x].l);
 
        if (t[x].r && t[t[x].r].minPrevLow <= v) {
            int res = findLastLowPrevLE(t[x].r, v, add + lsz + 1);
            if (res != -1) return res;
        }
 
        if (t[x].r && t[t[x].r].firstType == LOW && t[x].val <= v) return add + lsz + 1;
        if (t[x].l && t[x].type == LOW && t[t[x].l].lastVal <= v) return add + lsz;
 
        return findLastLowPrevLE(t[x].l, v, add);
    }
 
    int findLastHighPrevGE(int x, int v, int add = 0) const {
        if (!x || t[x].maxPrevHigh < v) return -1;
        int lsz = sz(t[x].l);
 
        if (t[x].r && t[t[x].r].maxPrevHigh >= v) {
            int res = findLastHighPrevGE(t[x].r, v, add + lsz + 1);
            if (res != -1) return res;
        }
 
        if (t[x].r && t[t[x].r].firstType == HIGH && t[x].val >= v) return add + lsz + 1;
        if (t[x].l && t[x].type == HIGH && t[t[x].l].lastVal >= v) return add + lsz;
 
        return findLastHighPrevGE(t[x].l, v, add);
    }
 
    int findFirstLowNextLE(int x, int v, int add = 0) const {
        if (!x || t[x].minNextLow > v) return -1;
        int lsz = sz(t[x].l);
 
        if (t[x].l && t[t[x].l].minNextLow <= v) {
            int res = findFirstLowNextLE(t[x].l, v, add);
            if (res != -1) return res;
        }
 
        if (t[x].l && t[t[x].l].lastType == LOW && t[x].val <= v) return add + lsz - 1;
        if (t[x].r && t[x].type == LOW && t[t[x].r].firstVal <= v) return add + lsz;
 
        return findFirstLowNextLE(t[x].r, v, add + lsz + 1);
    }
 
    int findFirstHighNextGE(int x, int v, int add = 0) const {
        if (!x || t[x].maxNextHigh < v) return -1;
        int lsz = sz(t[x].l);
 
        if (t[x].l && t[t[x].l].maxNextHigh >= v) {
            int res = findFirstHighNextGE(t[x].l, v, add);
            if (res != -1) return res;
        }
 
        if (t[x].l && t[t[x].l].lastType == HIGH && t[x].val >= v) return add + lsz - 1;
        if (t[x].r && t[x].type == HIGH && t[t[x].r].firstVal >= v) return add + lsz;
 
        return findFirstHighNextGE(t[x].r, v, add + lsz + 1);
    }
};
 
inline Pool pool;
 
struct Summary {
    int len = 0;
    int mn = INFV, mx = NINFV;
    int fmn = 0, lmn = 0, fmx = 0, lmx = 0;
    int Pmin = 0, Pmax = 0, Smin = 0, Smax = 0;
    int ans = 0;
};
 
inline void releaseSummary(Summary &S) {
    pool.release(S.Pmin);
    pool.release(S.Pmax);
    pool.release(S.Smin);
    pool.release(S.Smax);
    S = Summary();
}
 
inline Summary copySummary(const Summary &A) {
    Summary B = A;
    pool.retain(B.Pmin);
    pool.retain(B.Pmax);
    pool.retain(B.Smin);
    pool.retain(B.Smax);
    return B;
}
 
inline int attachLeft(const Summary &A, int q) {
    int t1 = pool.findLastLowPrevLE(q, A.mx);
    int t2 = pool.findLastHighPrevGE(q, A.mn);
    int t = max(t1, t2);
    if (t < 0) t = 0;
 
    int tp = pool.typeAt(q, t);
    int suffix;
    if (t == 0) {
        suffix = pool.hold(q);
    } else {
        auto pr = pool.splitTreap(q, t);
        pool.release(pr.first);
        suffix = pr.second;
    }
 
    int left = (tp == LOW ? A.Pmax : A.Pmin);
    int res = pool.mergeTreap(left, suffix);
    pool.release(suffix);
    return res;
}
 
inline int attachRight(int q, const Summary &B) {
    int t1 = pool.findFirstLowNextLE(q, B.mx);
    int t2 = pool.findFirstHighNextGE(q, B.mn);
    int t;
    if (t1 == -1) t = t2;
    else if (t2 == -1) t = t1;
    else t = min(t1, t2);
    if (t < 0) t = pool.sz(q) - 1;
 
    int tp = pool.typeAt(q, t);
    int prefix;
    if (t == pool.sz(q) - 1) {
        prefix = pool.hold(q);
    } else {
        auto pr = pool.splitTreap(q, t + 1);
        prefix = pr.first;
        pool.release(pr.second);
    }
 
    int right = (tp == LOW ? B.Smax : B.Smin);
    int res = pool.mergeTreap(prefix, right);
    pool.release(prefix);
    return res;
}
 
inline int calcAns(const Summary &C) {
    if (C.len <= 1) return C.len;
    int best = INT_MAX;
    if (C.fmn <= C.lmx) best = min(best, pool.sz(C.Pmin) + pool.sz(C.Smax));
    if (C.fmx <= C.lmn) best = min(best, pool.sz(C.Pmax) + pool.sz(C.Smin));
    return best;
}
 
inline Summary makeLeaf(int v) {
    Summary S;
    S.len = 1;
    S.mn = S.mx = v;
    S.fmn = S.lmn = S.fmx = S.lmx = 0;
    S.Pmin = pool.newLeaf(v, LOW);
    S.Pmax = pool.newLeaf(v, HIGH);
    S.Smin = pool.newLeaf(v, LOW);
    S.Smax = pool.newLeaf(v, HIGH);
    S.ans = 1;
    return S;
}
 
inline Summary mergeSummary(const Summary &A, const Summary &B) {
    if (A.len == 0) return copySummary(B);
    if (B.len == 0) return copySummary(A);
 
    Summary C;
    C.len = A.len + B.len;
    C.mn = min(A.mn, B.mn);
    C.mx = max(A.mx, B.mx);
 
    C.fmn = (A.mn <= B.mn ? A.fmn : A.len + B.fmn);
    C.lmn = (B.mn <= A.mn ? A.len + B.lmn : A.lmn);
    C.fmx = (A.mx >= B.mx ? A.fmx : A.len + B.fmx);
    C.lmx = (B.mx >= A.mx ? A.len + B.lmx : A.lmx);
 
    C.Pmin = (A.mn <= B.mn ? pool.hold(A.Pmin) : attachLeft(A, B.Pmin));
    C.Pmax = (A.mx >= B.mx ? pool.hold(A.Pmax) : attachLeft(A, B.Pmax));
    C.Smin = (B.mn <= A.mn ? pool.hold(B.Smin) : attachRight(A.Smin, B));
    C.Smax = (B.mx >= A.mx ? pool.hold(B.Smax) : attachRight(A.Smax, B));
 
    C.ans = calcAns(C);
    return C;
}
 
struct SegTree {
    int n;
    vector<Summary> tr;
 
    SegTree() : n(0) {}
 
    explicit SegTree(const vector<int> &a) { init(a); }
 
    void init(const vector<int> &a) {
        n = (int)a.size() - 1;
        pool.reset(max<size_t>(1 << 20, min<size_t>(3000000, (size_t)n * 12 + 100000)));
        tr.assign(4 * (n + 5), Summary());
        build(1, 1, n, a);
    }
 
    void build(int u, int l, int r, const vector<int> &a) {
        if (l == r) {
            tr[u] = makeLeaf(a[l]);
            return;
        }
        int mid = (l + r) >> 1;
        build(u << 1, l, mid, a);
        build(u << 1 | 1, mid + 1, r, a);
        tr[u] = mergeSummary(tr[u << 1], tr[u << 1 | 1]);
    }
 
    void update(int u, int l, int r, int p, int v) {
        if (l == r) {
            releaseSummary(tr[u]);
            tr[u] = makeLeaf(v);
            return;
        }
        int mid = (l + r) >> 1;
        if (p <= mid) update(u << 1, l, mid, p, v);
        else update(u << 1 | 1, mid + 1, r, p, v);
 
        Summary ns = mergeSummary(tr[u << 1], tr[u << 1 | 1]);
        releaseSummary(tr[u]);
        tr[u] = ns;
    }
 
    void collect(int u, int l, int r, int ql, int qr, vector<int> &ids) const {
        if (ql <= l && r <= qr) {
            ids.push_back(u);
            return;
        }
        int mid = (l + r) >> 1;
        if (ql <= mid) collect(u << 1, l, mid, ql, qr, ids);
        if (qr > mid) collect(u << 1 | 1, mid + 1, r, ql, qr, ids);
    }
 
    int query(int l, int r) const {
        vector<int> ids;
        ids.reserve(64);
        collect(1, 1, n, l, r, ids);
 
        Summary res;
        for (int id : ids) {
            Summary ns = mergeSummary(res, tr[id]);
            releaseSummary(res);
            res = ns;
        }
 
        int ret = res.ans;
        releaseSummary(res);
        return ret;
    }
 
    void update(int p, int v) { update(1, 1, n, p, v); }
};
 
} // namespace extreme_fold
 
#ifndef EXTREME_FOLD_LIBRARY
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int n, q;
    cin >> n >> q;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i];
 
    extreme_fold::SegTree st(a);
 
    while (q--) {
        int opt;
        cin >> opt;
        if (opt == 1) {
            int x, y;
            cin >> x >> y;
            st.update(x, y);
        } else {
            int L, R;
            cin >> L >> R;
            cout << st.query(L, R) << '\n';
        }
    }
    return 0;
}
#endif
