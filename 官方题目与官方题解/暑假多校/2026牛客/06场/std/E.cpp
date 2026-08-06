#include <bits/stdc++.h>
using namespace std;

using u64 = uint64_t;

struct Info {
    int len = 0, mx = -1;
    u64 h = 0;
};

constexpr u64 BASE = 11995408973635179863ULL;
vector<u64> pw, rep;

void initHash(int n) {
    pw.assign(n + 1, 1);
    rep.assign(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        pw[i] = pw[i - 1] * BASE;
        rep[i] = rep[i - 1] * BASE + 1;
    }
}

Info join(const Info &a, const Info &b) {
    return {a.len + b.len, max(a.mx, b.mx), a.h * pw[b.len] + b.h};
}

Info runInfo(int value, int len) {
    return len ? Info{len, value, (u64(value) + 1) * rep[len]} : Info{};
}

struct PosTree {
    struct Node {
        int l = 0, r = 0;
        Info a{};
    };

    int n;
    vector<Node> tr;

    explicit PosTree(int n_) : n(n_) {
        tr.reserve(max(2, n * 20));
        tr.push_back({});
    }

    int add(int old, int q, int value, int lo, int hi) {
        int z = int(tr.size());
        tr.push_back(tr[old]);

        if (hi - lo == 1) {
            tr[z].a = runInfo(value, 1);
            return z;
        }

        int md = (lo + hi) >> 1;
        if (q < md) {
            tr[z].l = add(tr[old].l, q, value, lo, md);
        } else {
            tr[z].r = add(tr[old].r, q, value, md, hi);
        }

        tr[z].a = join(tr[tr[z].l].a, tr[tr[z].r].a);
        return z;
    }

    int add(int old, int q, int value) { return add(old, q, value, 0, n); }

    int countPosPrefix(int z, int q, int lo, int hi) const {
        if (!z || q <= lo) return 0;
        if (hi <= q) return tr[z].a.len;

        int md = (lo + hi) >> 1;
        return countPosPrefix(tr[z].l, q, lo, md) + countPosPrefix(tr[z].r, q, md, hi);
    }

    int countPosPrefix(int z, int q) const { return countPosPrefix(z, q, 0, n); }

    Info queryRank(int z, int ql, int qr) const {
        if (!z || ql >= qr) return {};

        const int sz = tr[z].a.len;
        if (ql == 0 && qr == sz) return tr[z].a;

        int ls = tr[tr[z].l].a.len;
        Info ans;

        if (ql < ls) {
            ans = queryRank(tr[z].l, ql, min(qr, ls));
        }
        if (qr > ls) {
            ans = join(ans, queryRank(tr[z].r, max(0, ql - ls), qr - ls));
        }

        return ans;
    }

    int valueAtRank(int z, int k) const {
        while (tr[z].a.len > 1) {
            int ls = tr[tr[z].l].a.len;
            if (k < ls) {
                z = tr[z].l;
            } else {
                k -= ls;
                z = tr[z].r;
            }
        }
        return tr[z].a.mx;
    }

    int positionAtRank(int z, int k) const {
        int lo = 0, hi = n;
        while (hi - lo > 1) {
            int md = (lo + hi) >> 1;
            int ls = tr[tr[z].l].a.len;
            if (k < ls)
                z = tr[z].l, hi = md;
            else
                k -= ls, z = tr[z].r, lo = md;
        }
        return lo;
    }

    int firstGreaterRank(int z, int ql, int qr, int value) const {
        if (!z || ql >= qr || tr[z].a.mx <= value) return -1;

        int sz = tr[z].a.len;
        if (sz == 1) return 0;

        int ls = tr[tr[z].l].a.len;
        if (ql < ls) {
            int got = firstGreaterRank(tr[z].l, ql, min(qr, ls), value);
            if (got != -1) return got;
        }

        if (qr > ls) {
            int got = firstGreaterRank(tr[z].r, max(0, ql - ls), qr - ls, value);
            if (got != -1) return ls + got;
        }

        return -1;
    }
};

struct CntTree {
    struct Node {
        int l = 0, r = 0, cnt = 0;
    };

    int sigma;
    vector<Node> tr;

    explicit CntTree(int sigma_) : sigma(sigma_) { tr.push_back({}); }

    int update(int old, int q, int delta, int lo, int hi) {
        int z = int(tr.size());
        tr.push_back(tr[old]);
        tr[z].cnt += delta;

        if (hi - lo == 1) return z;

        int md = (lo + hi) >> 1;
        if (q < md) {
            tr[z].l = update(tr[old].l, q, delta, lo, md);
        } else {
            tr[z].r = update(tr[old].r, q, delta, md, hi);
        }

        return z;
    }

    int update(int old, int q, int delta) { return update(old, q, delta, 0, sigma); }

    int countLess(int z, int q, int lo, int hi) const {
        if (!z || q <= lo) return 0;
        if (hi <= q) return tr[z].cnt;

        int md = (lo + hi) >> 1;
        return countLess(tr[z].l, q, lo, md) + countLess(tr[z].r, q, md, hi);
    }

    int countLess(int z, int q) const { return countLess(z, q, 0, sigma); }

    int rawValueAtRank(int z, int k) const {
        int lo = 0, hi = sigma;

        while (hi - lo > 1) {
            int md = (lo + hi) >> 1;
            int ls = tr[tr[z].l].cnt;

            if (k < ls) {
                z = tr[z].l;
                hi = md;
            } else {
                k -= ls;
                z = tr[z].r;
                lo = md;
            }
        }

        return lo;
    }

    int valueAtRank(int z, int k) const { return min(rawValueAtRank(z, k) + 1, sigma - 1); }

    int countMappedLess(int z, int value) const {
        if (value >= sigma) return tr[z].cnt;
        return countLess(z, value - 1);
    }
};

struct BagHeap {
    struct Piece {
        int type = 0;
        int x = 0, start = 0, len = 0;
    };

    struct Node {
        int l = 0, r = 0, dist = 1, key = 0;
        Piece piece{};
    };

    const CntTree &pst;
    vector<Node> tr;

    explicit BagHeap(const CntTree &pst_) : pst(pst_) {
        tr.push_back({});
        tr[0].dist = 0;
    }

    int pieceMin(const Piece &p) const { return p.type == 0 ? p.x : pst.valueAtRank(p.x, p.start); }

    int meld(int a, int b) {
        if (!a || !b) return a ? a : b;
        if (tr[a].key > tr[b].key) swap(a, b);

        tr[a].r = meld(tr[a].r, b);
        if (tr[tr[a].l].dist < tr[tr[a].r].dist) {
            swap(tr[a].l, tr[a].r);
        }
        tr[a].dist = tr[tr[a].r].dist + 1;

        return a;
    }

    int addPiece(int root, Piece p) {
        if (!p.len) return root;

        int z = int(tr.size());
        tr.push_back({0, 0, 1, pieceMin(p), p});
        return meld(root, z);
    }

    int addRun(int root, int value, int count) { return addPiece(root, {0, value, 0, count}); }

    int addHist(int root, int histRoot) {
        int len = histRoot ? pst.tr[histRoot].cnt : 0;
        return addPiece(root, {1, histRoot, 0, len});
    }
};

struct SeqTree {
    struct Node {
        int l = 0, r = 0;
        unsigned pri = 0;
        int type = 0;
        int x = 0, y = 0;
        Info one{}, all{};
    };

    const PosTree &pst;
    vector<Node> tr;
    u64 rngState = 0x243f6a8885a308d3ULL;

    explicit SeqTree(const PosTree &pst_) : pst(pst_) { tr.push_back({}); }

    unsigned rnd() {
        rngState ^= rngState << 7;
        rngState ^= rngState >> 9;
        return unsigned(rngState);
    }

    int alloc() {
        tr.push_back({});
        return int(tr.size()) - 1;
    }

    Info all(int z) const { return z ? tr[z].all : Info{}; }

    void pull(int z) { tr[z].all = join(join(all(tr[z].l), tr[z].one), all(tr[z].r)); }

    int makeRun(int value, int count) {
        if (!count) return 0;

        int z = alloc();
        tr[z].pri = rnd();
        tr[z].type = 0;
        tr[z].x = value;
        tr[z].y = 0;
        tr[z].one = runInfo(value, count);
        tr[z].all = tr[z].one;

        return z;
    }

    int makeSlice(int root, int start, int len) {
        if (!len) return 0;

        int z = alloc();
        tr[z].pri = rnd();
        tr[z].type = 1;
        tr[z].x = root;
        tr[z].y = start;
        tr[z].one = pst.queryRank(root, start, start + len);
        tr[z].all = tr[z].one;

        return z;
    }

    int merge(int a, int b) {
        if (!a || !b) return a ? a : b;

        if (tr[a].pri > tr[b].pri) {
            tr[a].r = merge(tr[a].r, b);
            pull(a);
            return a;
        }

        tr[b].l = merge(a, tr[b].l);
        pull(b);
        return b;
    }

    int firstGreaterFrom(int z, int skip, int value) const {
        if (!z || skip >= tr[z].all.len || tr[z].all.mx <= value) {
            return -1;
        }

        int ls = all(tr[z].l).len;
        if (skip < ls) {
            int got = firstGreaterFrom(tr[z].l, skip, value);
            if (got != -1) return got;
        }

        int chunkSkip = max(0, skip - ls);
        if (chunkSkip < tr[z].one.len && tr[z].one.mx > value) {
            int got = -1;

            if (tr[z].type == 0) {
                got = tr[z].x > value ? chunkSkip : -1;
            } else {
                int q = pst.firstGreaterRank(tr[z].x, tr[z].y + chunkSkip, tr[z].y + tr[z].one.len,
                                             value);
                if (q != -1) got = q - tr[z].y;
            }

            if (got != -1) return ls + got;
        }

        int rightSkip = max(0, skip - ls - tr[z].one.len);
        int got = firstGreaterFrom(tr[z].r, rightSkip, value);

        return got == -1 ? -1 : ls + tr[z].one.len + got;
    }

    int prependRun(int root, int value, int count = 1) {
        return merge(makeRun(value, count), root);
    }

    Info prefix(int z, int k) const {
        if (!z || k <= 0) return {};
        if (k >= tr[z].all.len) return tr[z].all;

        int ls = all(tr[z].l).len;
        if (k <= ls) return prefix(tr[z].l, k);

        Info ans = all(tr[z].l);
        int take = min(k - ls, tr[z].one.len);
        Info mid;

        if (take == tr[z].one.len) {
            mid = tr[z].one;
        } else if (tr[z].type == 0) {
            mid = runInfo(tr[z].x, take);
        } else {
            mid = pst.queryRank(tr[z].x, tr[z].y, tr[z].y + take);
        }

        ans = join(ans, mid);

        if (k > ls + tr[z].one.len) {
            ans = join(ans, prefix(tr[z].r, k - ls - tr[z].one.len));
        }

        return ans;
    }

    u64 rangeHash(int z, int l, int r) const {
        if (l >= r) return {};

        u64 a = prefix(z, l).h;
        u64 b = prefix(z, r).h;
        int len = r - l;
        return b - a * pw[len];
    }

    int at(int z, int k) const {
        int ls = all(tr[z].l).len;
        if (k < ls) return at(tr[z].l, k);

        k -= ls;
        if (k < tr[z].one.len) {
            if (tr[z].type == 0) return tr[z].x;
            return pst.valueAtRank(tr[z].x, tr[z].y + k);
        }

        return at(tr[z].r, k - tr[z].one.len);
    }
};

struct Engine {
    struct State {
        int l = 0, v = 0, r = 0;
    };

    struct Part {
        int cut = 0, pre = 0, suf = 0;
    };

    struct Split {
        int prefix = 0, suffix = 0;
        Part keepPart{}, movePart{};
    };

    struct Answer {
        int seqRoot = 0, bag = 0, used = 0;
    };

    int n, m;
    vector<int> a;
    vector<vector<int>> occ, ver;
    PosTree pos;
    CntTree bags;
    SeqTree seq;
    BagHeap heap;

    Engine(vector<int> a_, int m_)
        : n(int(a_.size())), m(m_), a(move(a_)), occ(m + 1), ver(m + 1), pos(n), bags(m + 1),
          seq(pos), heap(bags) {
        for (int i = 0; i < n; ++i) {
            occ[a[i]].push_back(i);
        }

        int higher = 0;
        for (int v = m; v >= 0; --v) {
            ver[v].reserve(occ[v].size() + 1);
            ver[v].push_back(higher);

            int cur = higher;
            for (int p : occ[v]) {
                cur = pos.add(cur, p, v);
                ver[v].push_back(cur);
            }
            higher = cur;
        }
    }

    State normalize(State s, int hist) const {
        if (!bags.tr[hist].cnt) return {n, m + 1, 0};
        int nv = bags.rawValueAtRank(hist, 0);
        if (nv != s.v) s.v = nv, s.r = int(occ[nv].size());
        return s;
    }

    int versionRoot(const State &s) const { return s.v <= m ? ver[s.v][s.r] : 0; }

    int startRank(const State &s) const {
        return s.l >= n ? 0 : pos.countPosPrefix(versionRoot(s), s.l);
    }

    int activeLen(const State &s) const {
        if (s.l >= n || s.v > m) return 0;

        int rt = versionRoot(s);
        return pos.tr[rt].a.len - pos.countPosPrefix(rt, s.l);
    }

    int activeRankBefore(const State &s, int originalPos) const {
        int rt = versionRoot(s);
        return pos.countPosPrefix(rt, originalPos) - pos.countPosPrefix(rt, s.l);
    }

    int originalPosAtActiveRank(const State &s, int rank) const {
        return pos.positionAtRank(versionRoot(s), startRank(s) + rank);
    }

    int makeActiveWord(const State &s) {
        int rt = versionRoot(s);
        int st = startRank(s);
        int len = pos.tr[rt].a.len - st;
        return seq.makeSlice(rt, st, len);
    }

    int histAdd(int root, int value, int delta) {
        return delta ? bags.update(root, value, delta) : root;
    }

    vector<int> activeValues(const State &s, int l, int r) const {
        vector<int> z;
        if (l >= r) return z;

        int rt = versionRoot(s);
        int ql = pos.countPosPrefix(rt, l);
        int qr = pos.countPosPrefix(rt, r);

        z.reserve(qr - ql);
        for (int i = ql; i < qr; ++i) {
            z.push_back(pos.valueAtRank(rt, i));
        }

        return z;
    }

    int addValues(int root, const vector<int> &z, int delta) {
        for (int x : z) {
            root = histAdd(root, x, delta);
        }
        return root;
    }

    int buildValues(const vector<int> &z) { return addValues(0, z, 1); }

    Split splitAt(const State &s, int hist, const Part &c, int y) {
        const int val = a[y];
        Split out;

        if (y < c.cut) {
            int p = activeRankBefore(s, y);
            int x = activeRankBefore(s, c.cut) - p - 1;

            if (p <= x) {
                vector<int> v = activeValues(s, s.l, y);
                out.prefix = buildValues(v);

                int noPrefix = addValues(hist, v, -1);
                out.suffix = histAdd(noPrefix, val, -1);

                int middle = histAdd(addValues(c.pre, v, -1), val, -1);

                out.keepPart = {c.cut, middle, c.suf};
                out.movePart = {y, out.prefix, noPrefix};
            } else {
                vector<int> v = activeValues(s, y + 1, c.cut);
                int middle = buildValues(v);

                out.prefix = histAdd(addValues(c.pre, v, -1), val, -1);
                out.suffix = addValues(c.suf, v, 1);

                int anchorAndSuffix = histAdd(out.suffix, val, 1);

                out.keepPart = {c.cut, middle, c.suf};
                out.movePart = {y, out.prefix, anchorAndSuffix};
            }
        } else if (y == c.cut) {
            out.prefix = c.pre;
            out.suffix = histAdd(c.suf, val, -1);
            out.keepPart = {y + 1, 0, out.suffix};
            out.movePart = {y, out.prefix, c.suf};
        } else {
            int d = activeRankBefore(s, y) - activeRankBefore(s, c.cut);
            int after = activeLen(s) - activeRankBefore(s, y) - 1;

            if (d <= after) {
                vector<int> v = activeValues(s, c.cut, y);
                out.prefix = addValues(c.pre, v, 1);

                int noMiddle = addValues(c.suf, v, -1);
                out.suffix = histAdd(noMiddle, val, -1);

                out.keepPart = {y + 1, 0, out.suffix};
                out.movePart = {y, out.prefix, noMiddle};
            } else {
                vector<int> v = activeValues(s, y + 1, n);
                out.suffix = buildValues(v);
                out.prefix = histAdd(addValues(hist, v, -1), val, -1);

                int anchorAndSuffix = histAdd(out.suffix, val, 1);

                out.keepPart = {y + 1, 0, out.suffix};
                out.movePart = {y, out.prefix, anchorAndSuffix};
            }
        }

        return out;
    }

    pair<int, Part> deleteLastMin(const State &s, int hist, Part c, int q) {
        int from = s.r - q;
        int cut = int(lower_bound(occ[s.v].begin(), occ[s.v].end(), c.cut) - occ[s.v].begin());

        int qPre = clamp(min(s.r, cut) - from, 0, q);

        hist = histAdd(hist, s.v, -q);
        c.pre = histAdd(c.pre, s.v, -qPre);
        c.suf = histAdd(c.suf, s.v, -(q - qPre));

        return {hist, c};
    }

    pair<int, Part> removeLeading(const State &s, int hist, Part c, int take, int newL) {
        int firstRank = int(lower_bound(occ[s.v].begin(), occ[s.v].end(), s.l) - occ[s.v].begin());
        int cut = int(lower_bound(occ[s.v].begin(), occ[s.v].end(), c.cut) - occ[s.v].begin());

        int qPre = clamp(cut - firstRank, 0, take);

        hist = histAdd(hist, s.v, -take);
        c.pre = histAdd(c.pre, s.v, -qPre);
        c.suf = histAdd(c.suf, s.v, -(take - qPre));

        if (newL > c.cut) {
            c = {newL, 0, hist};
        }

        return {hist, c};
    }

    struct Chunk {
        int type = 0, x = 0, start = 0, len = 0;
    };

    struct BagCursor {
        struct Entry {
            int key = 0, heapNode = 0;
            BagHeap::Piece piece{};
        };

        struct Greater {
            bool operator()(const Entry &a, const Entry &b) const { return a.key > b.key; }
        };

        const Engine &e;
        priority_queue<Entry, vector<Entry>, Greater> pq;

        BagCursor(const Engine &e_, int root) : e(e_) {
            if (root) pushHeap(root);
        }

        void pushHeap(int z) {
            if (z) pq.push({e.heap.tr[z].key, z, {}});
        }

        void pushPiece(BagHeap::Piece p) {
            if (p.len) pq.push({e.heap.pieceMin(p), 0, p});
        }

        void expandOne() {
            Entry q = pq.top();
            pq.pop();

            const auto &z = e.heap.tr[q.heapNode];
            pushHeap(z.l);
            pushHeap(z.r);
            pushPiece(z.piece);
        }

        int peek() {
            while (!pq.empty() && pq.top().heapNode) expandOne();

            return pq.empty() ? INT_MAX : pq.top().key;
        }

        pair<int, int> consumeRun() {
            int value = peek();
            int total = 0;

            while (!pq.empty() && pq.top().key == value) {
                if (pq.top().heapNode) {
                    expandOne();
                    continue;
                }

                Entry q = pq.top();
                pq.pop();
                auto p = q.piece;
                int take;

                if (p.type == 0) {
                    take = p.len;
                } else {
                    int cut = e.bags.countMappedLess(p.x, value + 1);
                    take = min(p.start + p.len, cut) - p.start;
                }

                total += take;
                p.start += take;
                p.len -= take;
                pushPiece(p);
            }

            return {value, total};
        }
    };

    struct MergedCursor {
        const Engine &e;
        int seqRoot = 0, pos = 0, len = 0;
        BagCursor bag;

        MergedCursor(const Engine &e_, const Answer &r)
            : e(e_), seqRoot(r.seqRoot), len(e_.seq.all(r.seqRoot).len), bag(e_, r.bag) {}

        bool next(Chunk &out) {
            int b = bag.peek();

            if (pos < len) {
                if (b == INT_MAX) {
                    out = {0, seqRoot, pos, len - pos};
                    pos = len;
                    return true;
                }

                int p = e.seq.firstGreaterFrom(seqRoot, pos, b);

                if (p == -1) {
                    out = {0, seqRoot, pos, len - pos};
                    pos = len;
                    return true;
                }

                if (p > pos) {
                    out = {0, seqRoot, pos, p - pos};
                    pos = p;
                    return true;
                }
            }

            if (b == INT_MAX) return false;

            auto [value, count] = bag.consumeRun();
            out = {1, value, 0, count};
            return true;
        }
    };

    u64 chunkHash(const Chunk &c, int off, int len) const {
        return c.type == 0 ? seq.rangeHash(c.x, c.start + off, c.start + off + len)
                           : runInfo(c.x, len).h;
    }

    int chunkAt(const Chunk &c, int off) const {
        return c.type == 0 ? seq.at(c.x, c.start + off) : c.x;
    }

    static int compareResults(const Engine &a, const Answer &ra, const Engine &b,
                              const Answer &rb) {
        MergedCursor ia(a, ra), ib(b, rb);
        Chunk ca, cb;
        int pa = 0, pb = 0;
        bool ha = ia.next(ca);
        bool hb = ib.next(cb);

        while (ha && hb) {
            int take = min(ca.len - pa, cb.len - pb);

            u64 xa = a.chunkHash(ca, pa, take);
            u64 xb = b.chunkHash(cb, pb, take);

            if (xa != xb) {
                int lo = 0, hi = take;

                while (lo < hi) {
                    int md = (lo + hi + 1) >> 1;
                    if (a.chunkHash(ca, pa, md) == b.chunkHash(cb, pb, md)) {
                        lo = md;
                    } else {
                        hi = md - 1;
                    }
                }

                int va = a.chunkAt(ca, pa + lo);
                int vb = b.chunkAt(cb, pb + lo);
                return va < vb ? -1 : 1;
            }

            pa += take;
            pb += take;

            if (pa == ca.len) {
                pa = 0;
                ha = ia.next(ca);
            }
            if (pb == cb.len) {
                pb = 0;
                hb = ib.next(cb);
            }
        }

        if (ha != hb) return ha ? 1 : -1;
        return 0;
    }

    vector<int> materialize(const Answer &r) const {
        MergedCursor it(*this, r);
        Chunk c;
        vector<int> out;
        out.reserve(n);

        while (it.next(c)) {
            if (c.type == 1) {
                out.insert(out.end(), c.len, c.x);
            } else {
                for (int i = 0; i < c.len; ++i) {
                    out.push_back(seq.at(c.x, c.start + i));
                }
            }
        }

        return out;
    }

    Answer solveIter(State initial, int initialK, bool atMost, int initialHist, Part initialPart) {
        enum Stage { ENTER, PREFIX, KEEP, MOVE, ANCHOR, CHOOSE };
        struct Frame {
            State s{};
            Part part{}, escapePart{};
            int k = 0, hist = 0;
            Stage stage = ENTER;
            int value = 0, count = 0, bag = 0;
            int usedAdd = 0, q = 0, j = 0;
            Answer candidate{};
        };

        auto frame = [](State s, int k, int hist, Part part) {
            Frame f;
            f.s = s;
            f.k = k;
            f.hist = hist;
            f.part = part;
            return f;
        };

        vector<Frame> st;
        st.reserve(2 * n + 8);
        st.push_back(frame(initial, initialK, initialHist, initialPart));

        Answer last;

        while (!st.empty()) {
            Frame &f = st.back();

            if (PREFIX <= f.stage && f.stage <= MOVE) {
                if (f.stage == KEEP) last.bag = heap.addHist(last.bag, f.bag);
                if (f.stage == MOVE)
                    last.bag = heap.addRun(last.bag, f.value, f.count);
                else
                    last.seqRoot = seq.prependRun(last.seqRoot, f.value, f.count);
                last.used += f.usedAdd;
                st.pop_back();
                continue;
            }

            if (f.stage == ANCHOR) {
                Answer r = last;
                r.bag = heap.addHist(r.bag, f.bag);
                r.seqRoot = seq.prependRun(r.seqRoot, f.value);
                r.used += f.usedAdd;
                f.candidate = r;

                auto [moveHist, escapePart] = deleteLastMin(f.s, f.hist, f.escapePart, f.q);

                State ms{f.s.l, f.s.v, f.s.r - f.q};
                int childK = f.j - 1;

                f.stage = CHOOSE;
                st.push_back(frame(ms, childK, moveHist, escapePart));
                continue;
            }

            if (f.stage == CHOOSE) {
                Answer moved = last;
                moved.bag = heap.addRun(moved.bag, f.value, f.q);
                moved.used += f.q;

                Answer anchored = f.candidate;
                int cmp = compareResults(*this, anchored, *this, moved);

                if (cmp < 0 || (cmp == 0 && (!atMost || anchored.used >= moved.used))) {
                    last = anchored;
                } else {
                    last = moved;
                }

                st.pop_back();
                continue;
            }

            f.s = normalize(f.s, f.hist);
            int len = activeLen(f.s);

            if (atMost) {
                f.k = min(f.k, len);
            }

            if (!len) {
                last = {};
                st.pop_back();
                continue;
            }

            if (f.k == 0) {
                last = {makeActiveWord(f.s), 0, 0};
                st.pop_back();
                continue;
            }

            if (f.s.v == m) {
                last = {makeActiveWord(f.s), 0, f.k};
                st.pop_back();
                continue;
            }

            if (!atMost && f.k == len) {
                last = {0, heap.addHist(0, f.hist), f.k};
                st.pop_back();
                continue;
            }

            int lb =
                int(lower_bound(occ[f.s.v].begin(), occ[f.s.v].end(), f.s.l) - occ[f.s.v].begin());
            int firstV = occ[f.s.v][lb];
            int p = activeRankBefore(f.s, firstV);

            if (p == 0) {
                int rt = versionRoot(f.s);
                int sr = startRank(f.s);
                int fg = pos.firstGreaterRank(rt, sr, sr + len, f.s.v);

                int take = (fg == -1 ? len : fg - sr);
                if (!atMost) take = min(take, len - f.k);
                int lastPos = originalPosAtActiveRank(f.s, take - 1);

                auto [nextHist, nextPart] = removeLeading(f.s, f.hist, f.part, take, lastPos + 1);
                State ch{lastPos + 1, f.s.v, f.s.r};

                f.stage = PREFIX;
                f.value = f.s.v;
                f.count = take;
                st.push_back(frame(ch, f.k, nextHist, nextPart));
                continue;
            }

            if (p <= f.k) {
                Split sp = splitAt(f.s, f.hist, f.part, firstV);

                State ch{firstV + 1, f.s.v, f.s.r};
                int childK = f.k - p;

                f.stage = KEEP;
                f.value = f.s.v;
                f.count = 1;
                f.bag = sp.prefix;
                f.usedAdd = p;

                st.push_back(frame(ch, childK, sp.suffix, sp.keepPart));
                continue;
            }

            int w = f.s.v + 1;
            int cntV = f.s.r - lb;

            auto itw = lower_bound(occ[w].begin(), occ[w].end(), f.s.l);

            int firstW = (itw == occ[w].end() ? n : *itw);
            int j = (firstW == n ? len : activeRankBefore(f.s, firstW));

            if (j > f.k) {
                int q = min(cntV, f.k);

                auto [nextHist, nextPart] = deleteLastMin(f.s, f.hist, f.part, q);

                State ch{f.s.l, f.s.v, f.s.r - q};
                int childK = f.k - q;

                f.stage = MOVE;
                f.value = w;
                f.count = q;
                f.usedAdd = q;

                st.push_back(frame(ch, childK, nextHist, nextPart));
                continue;
            }

            Split sp = splitAt(f.s, f.hist, f.part, firstW);

            int q = f.k - j + 1;
            State rs{firstW + 1, f.s.v, f.s.r};
            int childK = f.k - j;

            f.value = w;
            f.bag = sp.prefix;
            f.usedAdd = j;

            if (q > f.k || cntV < q) {
                f.stage = KEEP;
                f.count = 1;
            } else {
                f.stage = ANCHOR;
                f.q = q;
                f.j = j;
                f.escapePart = sp.movePart;
            }

            st.push_back(frame(rs, childK, sp.suffix, sp.keepPart));
        }

        return last;
    }

    Answer solve(int k, bool atMost) {
        int v = *min_element(a.begin(), a.end());
        int hist = 0;
        for (int x : a) hist = histAdd(hist, x, 1);
        return solveIter({0, v, int(occ[v].size())}, k, atMost, hist, {0, 0, hist});
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n, k, m;
        cin >> n >> k >> m;
        vector<int> a(n);
        for (int &x : a) cin >> x;

        vector<int> ans;
        if (k == 0 || *min_element(a.begin(), a.end()) == m) {
            ans = a;
        } else if (n == 1) {
            ans = {min(m, a[0] + k)};
        } else {
            initHash(n);
            Engine ex(a, m);
            auto best = ex.solve(k, false);

            if (k < 2) {
                ans = ex.materialize(best);
            } else {
                auto it = max_element(a.begin(), a.end());
                int mx = *it, at = int(it - a.begin());
                vector<int> b = a;
                b.erase(b.begin() + at);

                Engine ep(move(b), m);
                auto cand = ep.solve(k - 2, true);
                int heavy = min(m, mx + k - cand.used);
                cand.bag = ep.heap.addRun(cand.bag, heavy, 1);

                if (Engine::compareResults(ex, best, ep, cand) > 0)
                    ans = ep.materialize(cand);
                else
                    ans = ex.materialize(best);
            }
        }

        for (int i = 0; i < n; ++i) cout << ans[i] << " \n"[i + 1 == n];
    }
}