#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
const int G = 3;

int mod_pow(long long a, long long e) {
    long long r = 1;
    while (e) {
        if (e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return (int)r;
}

vector<vector<int>> ntt_roots(1), ntt_inv_roots(1), bitrev(1);
vector<int> ntt_inv_len(1, 1);

int prepare_ntt(int n) {
    int lg = 0;
    while ((1 << lg) < n) lg++;

    while ((int)ntt_roots.size() <= lg) {
        int level = (int)ntt_roots.size();
        int len = 1 << level;
        int half = len >> 1;
        int wlen = mod_pow(G, (MOD - 1) / len);
        int iwlen = mod_pow(wlen, MOD - 2);

        ntt_roots.push_back(vector<int>(half));
        ntt_inv_roots.push_back(vector<int>(half));
        long long w = 1, iw = 1;
        for (int i = 0; i < half; i++) {
            ntt_roots[level][i] = (int)w;
            ntt_inv_roots[level][i] = (int)iw;
            w = w * wlen % MOD;
            iw = iw * iwlen % MOD;
        }
        ntt_inv_len.push_back(mod_pow(len, MOD - 2));
    }

    if ((int)bitrev.size() <= lg) bitrev.resize(lg + 1);
    if (bitrev[lg].empty()) {
        bitrev[lg].resize(n);
        if (lg > 0) {
            for (int i = 0; i < n; i++) {
                bitrev[lg][i] = (bitrev[lg][i >> 1] >> 1) | ((i & 1) << (lg - 1));
            }
        }
    }
    return lg;
}

void ntt(vector<int>& a, bool invert) {
    int n = (int)a.size();
    int lg = prepare_ntt(n);
    const vector<int>& rev = bitrev[lg];
    for (int i = 1; i < n; i++) {
        if (i < rev[i]) swap(a[i], a[rev[i]]);
    }
    for (int len = 2, level = 1; len <= n; len <<= 1, level++) {
        const vector<int>& ws = invert ? ntt_inv_roots[level] : ntt_roots[level];
        for (int i = 0; i < n; i += len) {
            int half = len >> 1;
            for (int j = 0; j < half; j++) {
                int u = a[i + j];
                int v = (int)((long long)a[i + j + half] * ws[j] % MOD);
                int x = u + v;
                if (x >= MOD) x -= MOD;
                int y = u - v;
                if (y < 0) y += MOD;
                a[i + j] = x;
                a[i + j + half] = y;
            }
        }
    }
    if (invert) {
        int inv_n = ntt_inv_len[lg];
        for (int& x : a) x = (int)((long long)x * inv_n % MOD);
    }
}

vector<int> multiply_poly(const vector<int>& a, const vector<int>& b) {
    if (a.empty() || b.empty()) return {};
    int need = (int)a.size() + (int)b.size() - 1;
    if (1LL * a.size() * b.size() <= 200000) {
        vector<int> c(need);
        for (int i = 0; i < (int)a.size(); i++) if (a[i]) {
            for (int j = 0; j < (int)b.size(); j++) if (b[j]) {
                c[i + j] = (c[i + j] + (long long)a[i] * b[j]) % MOD;
            }
        }
        return c;
    }
    int n = 1;
    while (n < need) n <<= 1;
    vector<int> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    fa.resize(n);
    fb.resize(n);
    ntt(fa, false);
    ntt(fb, false);
    for (int i = 0; i < n; i++) fa[i] = (int)((long long)fa[i] * fb[i] % MOD);
    ntt(fa, true);
    fa.resize(need);
    return fa;
}

void trim(vector<int>& a) {
    while (a.size() > 1 && a.back() == 0) a.pop_back();
}

vector<int> divide_by_monic(vector<int> a, const vector<int>& b) {
    trim(a);
    int n = (int)a.size() - 1;
    int m = (int)b.size() - 1;
    vector<int> q(max(0, n - m + 1));
    for (int i = n; i >= m; i--) {
        int coef = a[i];
        q[i - m] = coef;
        if (!coef) continue;
        int base = i - m;
        for (int j = 0; j <= m; j++) {
            a[base + j] -= (int)((long long)coef * b[j] % MOD);
            if (a[base + j] < 0) a[base + j] += MOD;
        }
    }
    trim(q);
    return q;
}

struct Solver {
    int n;
    vector<int> a;
    vector<vector<int>> cyclo;
    vector<int> pref_deg;
    vector<vector<int>> seg;
    vector<vector<int>> q_cache;
    vector<char> has_cache;
    unordered_map<int, vector<int>> a_ntt_cache;

    void build_cyclotomics() {
        cyclo.assign(1, vector<int>{});
        pref_deg.assign(1, 0);
        for (int m = 1; pref_deg.back() < n; m++) {
            vector<int> cur(m + 1, 0);
            cur[0] = MOD - 1;
            cur[m] = 1;
            for (int d = 1; d < m; d++) {
                if (m % d == 0) cur = divide_by_monic(cur, cyclo[d]);
            }
            trim(cur);
            cyclo.push_back(cur);
            pref_deg.push_back(pref_deg.back() + (int)cur.size() - 1);
        }
        int m = (int)cyclo.size() - 1;
        seg.assign(4 * m + 4, {});
        build_product_tree(1, 1, m);
        q_cache.assign(cyclo.size(), {});
        has_cache.assign(cyclo.size(), 0);
    }

    void build_product_tree(int v, int l, int r) {
        if (l == r) {
            seg[v] = cyclo[l];
            return;
        }
        int mid = (l + r) >> 1;
        build_product_tree(v << 1, l, mid);
        build_product_tree(v << 1 | 1, mid + 1, r);
        seg[v] = multiply_poly(seg[v << 1], seg[v << 1 | 1]);
    }

    void collect_segments(int v, int l, int r, int ql, int qr, vector<int>& nodes) {
        if (ql <= l && r <= qr) {
            nodes.push_back(v);
            return;
        }
        int mid = (l + r) >> 1;
        if (ql <= mid) collect_segments(v << 1, l, mid, ql, qr, nodes);
        if (mid < qr) collect_segments(v << 1 | 1, mid + 1, r, ql, qr, nodes);
    }

    vector<int> multiply_many(vector<vector<int>>& polys) {
        if (polys.empty()) return vector<int>{1};
        while (polys.size() > 1) {
            vector<vector<int>> nxt;
            nxt.reserve((polys.size() + 1) / 2);
            for (int i = 0; i < (int)polys.size(); i += 2) {
                if (i + 1 == (int)polys.size()) nxt.push_back(move(polys[i]));
                else nxt.push_back(multiply_poly(polys[i], polys[i + 1]));
            }
            polys.swap(nxt);
        }
        return move(polys[0]);
    }

    vector<int> build_Q(int t) {
        if (t == 0) return vector<int>{1};
        int m = (int)cyclo.size() - 1;
        vector<int> nodes;
        collect_segments(1, 1, m, 1, t, nodes);
        vector<vector<int>> cur;
        cur.reserve(nodes.size());
        for (int v : nodes) cur.push_back(seg[v]);
        return multiply_many(cur);
    }

    const vector<int>& get_Q(int t) {
        if (!has_cache[t]) {
            q_cache[t] = build_Q(t);
            has_cache[t] = 1;
        }
        return q_cache[t];
    }

    bool ok(int t) {
        int d = pref_deg[t];
        if (d >= n) return true;
        const vector<int>& q = get_Q(t);

        long long checks = n - d;
        auto check_start = [&](int start) {
            long long sum = 0;
            for (int j = 0; j <= d; j++) {
                sum += (long long)q[j] * a[start + j];
                if ((j & 7) == 7) sum %= MOD;
            }
            return sum % MOD == 0;
        };

        vector<int> probes;
        probes.reserve(6);
        probes.push_back(0);
        probes.push_back((int)checks - 1);
        probes.push_back((int)(checks >> 1));
        probes.push_back((int)(checks / 3));
        probes.push_back((int)(checks * 2 / 3));
        sort(probes.begin(), probes.end());
        probes.erase(unique(probes.begin(), probes.end()), probes.end());
        for (int start : probes) {
            if (!check_start(start)) return false;
        }

        long long direct_ops = checks * (long long)(d + 1);
        if (direct_ops <= 70000000LL) {
            for (int start = 0; start < checks; start++) {
                if (!check_start(start)) return false;
            }
            return true;
        }

        int len = 1;
        while (len < n + d) len <<= 1;
        auto it = a_ntt_cache.find(len);
        if (it == a_ntt_cache.end()) {
            vector<int> fa(a.begin(), a.end());
            fa.resize(len);
            ntt(fa, false);
            it = a_ntt_cache.emplace(len, move(fa)).first;
        }

        vector<int> fb(q.rbegin(), q.rend());
        fb.resize(len);
        ntt(fb, false);
        const vector<int>& fa = it->second;
        for (int i = 0; i < len; i++) fb[i] = (int)((long long)fb[i] * fa[i] % MOD);
        ntt(fb, true);
        for (int i = d; i < n; i++) {
            if (fb[i] != 0) return false;
        }
        return true;
    }

    int solve() {
        build_cyclotomics();
        int l = 0, r = (int)cyclo.size() - 1;
        while (l < r) {
            int mid = (l + r) >> 1;
            if (ok(mid)) r = mid;
            else l = mid + 1;
        }
        return l;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Solver solver;
    cin >> solver.n;
    solver.a.resize(solver.n);
    for (int i = 0; i < solver.n; i++) cin >> solver.a[i];
    cout << solver.solve() << '\n';
    return 0;
}
