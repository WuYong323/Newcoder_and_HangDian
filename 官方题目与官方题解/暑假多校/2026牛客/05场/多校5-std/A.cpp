// std.cpp - suffix array tree + HLD + lazy segment tree, O((n+q) log^2 n)
// Tag: AC
#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

struct SegTree {
    int n;
    vector<i64> sumCoef;
    vector<i64> lazy;
    vector<i64> sum;

    SegTree() = default;
    SegTree(const vector<i64>& coef, const vector<i64>& val) {
        n = (int)coef.size() - 1;
        sumCoef.assign(4 * (n + 1), 0);
        lazy.assign(4 * (n + 1), 0);
        sum.assign(4 * (n + 1), 0);
        build(1, 1, n, coef, val);
    }

    void build(int p, int l, int r, const vector<i64>& coef, const vector<i64>& val) {
        if (l == r) {
            sumCoef[p] = coef[l];
            sum[p] = (i64)coef[l] * val[l];
            return;
        }
        int m = (l + r) >> 1;
        build(p << 1, l, m, coef, val);
        build(p << 1 | 1, m + 1, r, coef, val);
        pull(p);
    }

    void pull(int p) {
        sumCoef[p] = sumCoef[p << 1] + sumCoef[p << 1 | 1];
        sum[p] = sum[p << 1] + sum[p << 1 | 1];
    }

    void apply(int p, i64 delta) {
        sum[p] += (i64)delta * sumCoef[p];
        lazy[p] += delta;
    }

    void push(int p) {
        if (lazy[p] != 0) {
            apply(p << 1, lazy[p]);
            apply(p << 1 | 1, lazy[p]);
            lazy[p] = 0;
        }
    }

    void add(int ql, int qr, i64 delta) { add(1, 1, n, ql, qr, delta); }

    void add(int p, int l, int r, int ql, int qr, i64 delta) {
        if (ql <= l && r <= qr) {
            apply(p, delta);
            return;
        }
        push(p);
        int m = (l + r) >> 1;
        if (ql <= m) add(p << 1, l, m, ql, qr, delta);
        if (qr > m) add(p << 1 | 1, m + 1, r, ql, qr, delta);
        pull(p);
    }

    i64 query(int ql, int qr) { return query(1, 1, n, ql, qr); }

    i64 query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return sum[p];
        push(p);
        int m = (l + r) >> 1;
        i64 res = 0;
        if (ql <= m) res += query(p << 1, l, m, ql, qr);
        if (qr > m) res += query(p << 1 | 1, m + 1, r, ql, qr);
        return res;
    }

    i64 pointVal(int pos) { return pointVal(1, 1, n, pos); }

    i64 pointVal(int p, int l, int r, int pos) {
        if (l == r) {
            return sumCoef[p] == 0 ? 0 : (i64)(sum[p] / sumCoef[p]);
        }
        push(p);
        int m = (l + r) >> 1;
        if (pos <= m) return pointVal(p << 1, l, m, pos);
        return pointVal(p << 1 | 1, m + 1, r, pos);
    }

    int lowerBound(int ql, int qr, i64& need) {
        return lowerBound(1, 1, n, ql, qr, need);
    }

    int lowerBound(int p, int l, int r, int ql, int qr, i64& need) {
        if (qr < l || r < ql) return -1;
        if (ql <= l && r <= qr && sum[p] < need) {
            need -= sum[p];
            return -1;
        }
        if (l == r) return l;
        push(p);
        int m = (l + r) >> 1;
        int res = lowerBound(p << 1, l, m, ql, qr, need);
        if (res != -1) return res;
        return lowerBound(p << 1 | 1, m + 1, r, ql, qr, need);
    }
};

vector<int> buildSA(const string& s) {
    int n = (int)s.size();
    vector<int> sa(n), rnk(n), tmp(n);
    iota(sa.begin(), sa.end(), 0);
    for (int i = 0; i < n; i++) rnk[i] = (unsigned char)s[i];
    for (int k = 1;; k <<= 1) {
        sort(sa.begin(), sa.end(), [&](int a, int b) {
            if (rnk[a] != rnk[b]) return rnk[a] < rnk[b];
            int ra = a + k < n ? rnk[a + k] : -1;
            int rb = b + k < n ? rnk[b + k] : -1;
            return ra < rb;
        });
        tmp[sa[0]] = 0;
        for (int i = 1; i < n; i++) {
            int a = sa[i - 1], b = sa[i];
            bool diff = rnk[a] != rnk[b];
            if (!diff) {
                int ra = a + k < n ? rnk[a + k] : -1;
                int rb = b + k < n ? rnk[b + k] : -1;
                diff = ra != rb;
            }
            tmp[b] = tmp[a] + diff;
        }
        rnk.swap(tmp);
        if (rnk[sa.back()] == n - 1) break;
    }
    return sa;
}

vector<int> buildLCP(const string& s, const vector<int>& sa) {
    int n = (int)s.size();
    vector<int> rank(n), lcp(n, 0);
    for (int i = 0; i < n; i++) rank[sa[i]] = i;
    int h = 0;
    for (int i = 0; i < n; i++) {
        int r = rank[i];
        if (r == 0) continue;
        int j = sa[r - 1];
        while (i + h < n && j + h < n && s[i + h] == s[j + h]) h++;
        lcp[r] = h;
        if (h) h--;
    }
    return lcp;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    string s;
    cin >> s;
    vector<i64> a(n);
    for (i64& x : a) cin >> x;

    string t = s;
    t.push_back(char(0));
    int N = n + 1;
    vector<int> sa = buildSA(t);
    vector<int> lcp = buildLCP(t, sa);

    vector<int> depth(1, 0), parent(1, -1), leaf(n, -1);
    auto newNode = [&](int dep) {
        depth.push_back(dep);
        parent.push_back(-1);
        return (int)depth.size() - 1;
    };

    vector<int> st;
    st.push_back(0);
    for (int i = 0; i < N; i++) {
        int l = (i == 0 ? 0 : lcp[i]);
        int last = -1;
        while (depth[st.back()] > l) {
            last = st.back();
            st.pop_back();
        }
        if (depth[st.back()] < l) {
            int v = newNode(l);
            parent[v] = st.back();
            if (last != -1) parent[last] = v;
            st.push_back(v);
        } else if (last != -1) {
            parent[last] = st.back();
        }

        int lf = newNode(N - sa[i]);
        if (sa[i] < n) leaf[sa[i]] = lf;
        parent[lf] = st.back();
        st.push_back(lf);
    }

    int V = (int)depth.size();
    vector<vector<int>> children(V);
    for (int v = 1; v < V; v++) children[parent[v]].push_back(v);

    vector<int> order;
    order.reserve(V);
    vector<int> stack = {0};
    while (!stack.empty()) {
        int v = stack.back();
        stack.pop_back();
        order.push_back(v);
        for (int u : children[v]) stack.push_back(u);
    }

    vector<int> sz(V, 1), heavy(V, -1);
    vector<i64> val(V, 0);
    for (int i = 0; i < n; i++) val[leaf[i]] = a[i];
    for (int i = V - 1; i >= 0; i--) {
        int v = order[i];
        sz[v] = 1;
        int best = 0;
        for (int u : children[v]) {
            sz[v] += sz[u];
            val[v] += val[u];
            if (sz[u] > best) {
                best = sz[u];
                heavy[v] = u;
            }
        }
    }

    vector<int> top(V), dfn(V), rev(V + 1);
    int timer = 0;
    vector<pair<int, int>> chains = {{0, 0}};
    while (!chains.empty()) {
        auto [start, tp] = chains.back();
        chains.pop_back();
        for (int v = start; v != -1; v = heavy[v]) {
            top[v] = tp;
            dfn[v] = ++timer;
            rev[timer] = v;
            for (int u : children[v]) {
                if (u != heavy[v]) chains.push_back({u, u});
            }
        }
    }

    int LOG = 1;
    while ((1 << LOG) <= V) LOG++;
    vector<vector<int>> up(LOG, vector<int>(V, 0));
    for (int v = 0; v < V; v++) up[0][v] = parent[v] == -1 ? 0 : parent[v];
    for (int k = 1; k < LOG; k++) {
        for (int v = 0; v < V; v++) up[k][v] = up[k - 1][up[k - 1][v]];
    }

    vector<i64> baseCoef(V + 1), baseVal(V + 1);
    for (int v = 0; v < V; v++) {
        baseCoef[dfn[v]] = parent[v] == -1 ? 0 : depth[v] - depth[parent[v]];
        baseVal[dfn[v]] = val[v];
    }
    SegTree seg(baseCoef, baseVal);

    auto pathAdd = [&](int v, i64 delta) {
        while (top[v] != 0) {
            seg.add(dfn[top[v]], dfn[v], delta);
            v = parent[top[v]];
        }
        seg.add(dfn[0], dfn[v], delta);
    };

    auto pathSum = [&](int v) {
        i64 ans = 0;
        while (top[v] != 0) {
            ans += seg.query(dfn[top[v]], dfn[v]);
            v = parent[top[v]];
        }
        ans += seg.query(dfn[0], dfn[v]);
        return ans;
    };

    auto locate = [&](int pos, int len) {
        int v = leaf[pos];
        for (int k = LOG - 1; k >= 0; k--) {
            int anc = up[k][v];
            if (depth[anc] >= len) v = anc;
        }
        return v;
    };

    auto calcB = [&](int pos, int len) {
        int c = locate(pos, len);
        int u = parent[c];
        i64 ans = pathSum(u);
        ans += (i64)seg.pointVal(dfn[c]) * (len - depth[u]);
        return ans;
    };

    auto printAnswer = [](i64 x) { cout << x << '\n'; };
    auto lowerAnswer = [&](int pos, int len, i64 k) {
        if (calcB(pos, len) < k) return 0;
        int c = locate(pos, len);
        int u = parent[c];
        vector<pair<int, int>> parts;
        int v = u;
        while (top[v] != 0) {
            parts.push_back({dfn[top[v]], dfn[v]});
            v = parent[top[v]];
        }
        parts.push_back({dfn[0], dfn[v]});
        reverse(parts.begin(), parts.end());

        i64 need = k;
        for (auto [l, r] : parts) {
            i64 total = seg.query(l, r);
            if (total < need) {
                need -= total;
                continue;
            }
            int posDfn = seg.lowerBound(l, r, need);
            int node = rev[posDfn];
            i64 curVal = seg.pointVal(posDfn);
            i64 step = (i64)((need + curVal - 1) / curVal);
            return depth[parent[node]] + (int)step;
        }

        i64 curVal = seg.pointVal(dfn[c]);
        i64 step = (i64)((need + curVal - 1) / curVal);
        return depth[parent[c]] + (int)step;
    };

    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int i;
            i64 x;
            cin >> i >> x;
            --i;
            i64 delta = x - a[i];
            a[i] = x;
            if (delta != 0) pathAdd(leaf[i], delta);
        } else if (type == 2) {
            int p, m;
            cin >> p >> m;
            printAnswer(calcB(p - 1, m));
        } else {
            int p, m;
            i64 k;
            cin >> p >> m >> k;
            cout << lowerAnswer(p - 1, m, k) << '\n';
        }
    }

    return 0;
}

