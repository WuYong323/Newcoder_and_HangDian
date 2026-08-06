#include <bits/stdc++.h>
#pragma GCC optimize("O3")
#pragma GCC target("popcnt")
using namespace std;
using ull = unsigned long long;

struct FastScanner {
    static const int S = 1 << 20;
    int p = 0, n = 0;
    char buf[S];
    inline char gc() {
        if (p == n) {
            n = (int)fread(buf, 1, S, stdin);
            p = 0;
            if (!n) return 0;
        }
        return buf[p++];
    }
    template<class T> bool readInt(T &x) {
        char c = gc();
        if (!c) return false;
        while (c <= ' ') {
            c = gc();
            if (!c) return false;
        }
        T sgn = 1;
        if (c == '-') sgn = -1, c = gc();
        T v = 0;
        while (c > ' ') v = v * 10 + (c - '0'), c = gc();
        x = v * sgn;
        return true;
    }
};

struct CSR {
    int n = 0;
    vector<int> st, to;
};

static CSR buildCSR(int n, const vector<int> &from, const vector<int> &toV) {
    CSR g;
    g.n = n;
    g.st.assign(n + 1, 0);
    for (int u : from) ++g.st[u + 1];
    for (int i = 0; i < n; ++i) g.st[i + 1] += g.st[i];
    g.to.assign(from.size(), 0);
    vector<int> cur = g.st;
    for (int i = 0; i < (int)from.size(); ++i) g.to[cur[from[i]]++] = toV[i];
    return g;
}

struct HopcroftKarp {
    int n, m;
    CSR g;
    vector<int> pu, pv, dist, it, q, stkU, stkV;
    static const int INF = 1000000000;
    HopcroftKarp(int n_, int m_, const vector<int> &eu, const vector<int> &ev) : n(n_), m(m_) {
        g = buildCSR(n, eu, ev);
        pu.assign(n, -1); pv.assign(m, -1); dist.resize(n); it.resize(n); q.resize(n);
        stkU.reserve(n); stkV.reserve(n);
    }
    bool bfs() {
        int h = 0, t = 0;
        for (int u = 0; u < n; ++u) {
            if (pu[u] < 0) dist[u] = 0, q[t++] = u;
            else dist[u] = INF;
        }
        bool ok = false;
        while (h < t) {
            int u = q[h++];
            for (int e = g.st[u]; e < g.st[u + 1]; ++e) {
                int v = g.to[e], u2 = pv[v];
                if (u2 < 0) ok = true;
                else if (dist[u2] == INF) dist[u2] = dist[u] + 1, q[t++] = u2;
            }
        }
        return ok;
    }
    bool dfsIter(int s) {
        stkU.clear(); stkV.clear(); stkU.push_back(s);
        while (!stkU.empty()) {
            int u = stkU.back();
            bool go = false;
            while (it[u] < g.st[u + 1]) {
                int v = g.to[it[u]++], u2 = pv[v];
                if (u2 < 0) {
                    stkV.push_back(v);
                    for (int i = (int)stkU.size() - 1; i >= 0; --i) {
                        pu[stkU[i]] = stkV[i];
                        pv[stkV[i]] = stkU[i];
                    }
                    return true;
                }
                if (dist[u2] == dist[u] + 1) {
                    stkV.push_back(v);
                    stkU.push_back(u2);
                    go = true;
                    break;
                }
            }
            if (go) continue;
            dist[u] = INF;
            stkU.pop_back();
            if (!stkU.empty() && (int)stkV.size() == (int)stkU.size()) stkV.pop_back();
        }
        return false;
    }
    int solve() {
        int ans = 0;
        while (bfs()) {
            for (int u = 0; u < n; ++u) it[u] = g.st[u];
            for (int u = 0; u < n; ++u) if (pu[u] < 0 && dfsIter(u)) ++ans;
        }
        return ans;
    }
};

struct SCCSolver {
    int n;
    const CSR *g, *rg;
    vector<int> comp;
    SCCSolver(const CSR &g_, const CSR &rg_) : n(g_.n), g(&g_), rg(&rg_) { comp.assign(n, -1); }
    int solve() {
        vector<unsigned char> vis(n, 0);
        vector<int> it(n), ord, st;
        ord.reserve(n); st.reserve(n);
        for (int s = 0; s < n; ++s) if (!vis[s]) {
            st.clear(); st.push_back(s); vis[s] = 1; it[s] = g->st[s];
            while (!st.empty()) {
                int u = st.back(), &e = it[u];
                if (e < g->st[u + 1]) {
                    int v = g->to[e++];
                    if (!vis[v]) vis[v] = 1, it[v] = g->st[v], st.push_back(v);
                } else ord.push_back(u), st.pop_back();
            }
        }
        int cc = 0;
        for (int i = n - 1; i >= 0; --i) {
            int s = ord[i];
            if (comp[s] != -1) continue;
            st.clear(); st.push_back(s); comp[s] = cc;
            while (!st.empty()) {
                int u = st.back(); st.pop_back();
                for (int e = rg->st[u]; e < rg->st[u + 1]; ++e) {
                    int v = rg->to[e];
                    if (comp[v] == -1) comp[v] = cc, st.push_back(v);
                }
            }
            ++cc;
        }
        return cc;
    }
};

static inline int pc(ull x) { return __builtin_popcountll(x); }
static inline void orWords(ull *dst, const ull *src, int W) {
    for (int i = 0; i < W; ++i) dst[i] |= src[i];
}

int main() {
    FastScanner fs;
    int n = 0, m = 0, e = 0, qn = 0;
    if (!fs.readInt(n)) return 0;
    fs.readInt(m); fs.readInt(e); fs.readInt(qn);
    const int V = n + m;

    vector<int> eu, ev;
    eu.reserve(e); ev.reserve(e);
    for (int i = 0; i < e; ++i) {
        int u = 0, v = 0;
        fs.readInt(u); fs.readInt(v);
        --u; v = v - n - 1;
        eu.push_back(u); ev.push_back(v);
    }

    HopcroftKarp hk(n, m, eu, ev);
    hk.solve();

    vector<int> df, dt, rf, rt;
    df.reserve(e + min(n, m)); dt.reserve(e + min(n, m));
    rf.reserve(e + min(n, m)); rt.reserve(e + min(n, m));
    auto addD = [&](int a, int b) { df.push_back(a); dt.push_back(b); rf.push_back(b); rt.push_back(a); };
    for (int i = 0; i < e; ++i) {
        int L = eu[i], R = n + ev[i];
        addD(L, R);
        if (hk.pu[eu[i]] == ev[i]) addD(R, L);
    }
    CSR D = buildCSR(V, df, dt), RD = buildCSR(V, rf, rt);
    vector<int>().swap(df); vector<int>().swap(dt); vector<int>().swap(rf); vector<int>().swap(rt);

    SCCSolver sc(D, RD);
    int K = sc.solve();
    vector<int> comp = move(sc.comp);

    vector<int> lc(K, 0), rc(K, 0);
    for (int i = 0; i < n; ++i) ++lc[comp[i]];
    for (int i = 0; i < m; ++i) ++rc[comp[n + i]];

    vector<unsigned long long> enc;
    enc.reserve(D.to.size());
    for (int u = 0; u < V; ++u) {
        int cu = comp[u];
        for (int p = D.st[u]; p < D.st[u + 1]; ++p) {
            int cv = comp[D.to[p]];
            if (cu != cv) enc.push_back((unsigned long long)cu * (unsigned long long)K + (unsigned int)cv);
        }
    }
    CSR().st.swap(D.st); CSR().to.swap(D.to); CSR().st.swap(RD.st); CSR().to.swap(RD.to);
    sort(enc.begin(), enc.end());
    enc.erase(unique(enc.begin(), enc.end()), enc.end());

    vector<int> gf, gt, grf, grt, indeg(K, 0);
    gf.reserve(enc.size()); gt.reserve(enc.size()); grf.reserve(enc.size()); grt.reserve(enc.size());
    for (auto z : enc) {
        int u = (int)(z / (unsigned long long)K), v = (int)(z % (unsigned long long)K);
        gf.push_back(u); gt.push_back(v); grf.push_back(v); grt.push_back(u); ++indeg[v];
    }
    vector<unsigned long long>().swap(enc);
    CSR dag = buildCSR(K, gf, gt), rdag = buildCSR(K, grf, grt);
    vector<int>().swap(gf); vector<int>().swap(gt); vector<int>().swap(grf); vector<int>().swap(grt);

    vector<int> topo, que(K);
    topo.reserve(K);
    int h = 0, t = 0;
    for (int i = 0; i < K; ++i) if (!indeg[i]) que[t++] = i;
    while (h < t) {
        int u = que[h++]; topo.push_back(u);
        for (int p = dag.st[u]; p < dag.st[u + 1]; ++p) if (--indeg[dag.to[p]] == 0) que[t++] = dag.to[p];
    }
    vector<int>().swap(indeg);

    vector<unsigned char> baseIn(K, 0), baseOut(K, 0);
    h = t = 0;
    for (int i = 0; i < n; ++i) if (hk.pu[i] < 0) { int c = comp[i]; if (!baseIn[c]) baseIn[c] = 1, que[t++] = c; }
    while (h < t) {
        int u = que[h++];
        for (int p = dag.st[u]; p < dag.st[u + 1]; ++p) { int v = dag.to[p]; if (!baseIn[v]) baseIn[v] = 1, que[t++] = v; }
    }
    h = t = 0;
    for (int i = 0; i < m; ++i) if (hk.pv[i] < 0) { int c = comp[n + i]; if (!baseOut[c]) baseOut[c] = 1, que[t++] = c; }
    while (h < t) {
        int u = que[h++];
        for (int p = rdag.st[u]; p < rdag.st[u + 1]; ++p) { int v = rdag.to[p]; if (!baseOut[v]) baseOut[v] = 1, que[t++] = v; }
    }

    bool globalBad = false;
    long long baseSel = 0, baseNot = 0;
    for (int c = 0; c < K; ++c) {
        if (baseIn[c] && baseOut[c]) globalBad = true;
        if (baseIn[c]) baseSel += rc[c], baseNot += lc[c];
        else if (baseOut[c]) baseSel += lc[c], baseNot += rc[c];
    }
    vector<int>().swap(hk.g.st); vector<int>().swap(hk.g.to); vector<int>().swap(hk.dist); vector<int>().swap(hk.it); vector<int>().swap(hk.q);

    vector<unsigned char> type(qn, 1), bad(qn, 0); // 0 bad, 1 empty, 2 one in, 3 one out, 4 general
    vector<int> oneComp(qn, -1), inSt(qn + 1, 0), outSt(qn + 1, 0), inList, outList, active;
    inList.reserve(300000); outList.reserve(300000); active.reserve(qn);
    vector<unsigned char> needIn(K, 0), needOut(K, 0);
    vector<int> oneIn, oneOut, tmpIn, tmpOut;

    for (int qi = 0; qi < qn; ++qi) {
        inSt[qi] = (int)inList.size(); outSt[qi] = (int)outList.size();
        int cnt = 0; fs.readInt(cnt);
        bool fail = globalBad;
        tmpIn.clear(); tmpOut.clear();
        for (int j = 0; j < cnt; ++j) {
            int x = 0, y = 0; fs.readInt(x); fs.readInt(y); --x;
            bool forceIn = (x < n ? (y == 1) : (y == 0));
            int c = comp[x];
            if (fail) continue;
            if (forceIn) {
                if (baseOut[c]) fail = true;
                else if (!baseIn[c]) tmpIn.push_back(c);
            } else {
                if (baseIn[c]) fail = true;
                else if (!baseOut[c]) tmpOut.push_back(c);
            }
        }
        if (!fail) {
            sort(tmpIn.begin(), tmpIn.end()); tmpIn.erase(unique(tmpIn.begin(), tmpIn.end()), tmpIn.end());
            sort(tmpOut.begin(), tmpOut.end()); tmpOut.erase(unique(tmpOut.begin(), tmpOut.end()), tmpOut.end());
            for (int i = 0, j = 0; i < (int)tmpIn.size() && j < (int)tmpOut.size();) {
                if (tmpIn[i] == tmpOut[j]) { fail = true; break; }
                if (tmpIn[i] < tmpOut[j]) ++i; else ++j;
            }
        }
        if (fail) type[qi] = 0, bad[qi] = 1;
        else {
            int tot = (int)tmpIn.size() + (int)tmpOut.size();
            if (tot == 0) type[qi] = 1;
            else if (tot == 1) {
                if (!tmpIn.empty()) {
                    type[qi] = 2; oneComp[qi] = tmpIn[0];
                    if (!needIn[tmpIn[0]]) needIn[tmpIn[0]] = 1, oneIn.push_back(tmpIn[0]);
                } else {
                    type[qi] = 3; oneComp[qi] = tmpOut[0];
                    if (!needOut[tmpOut[0]]) needOut[tmpOut[0]] = 1, oneOut.push_back(tmpOut[0]);
                }
            } else {
                type[qi] = 4;
                for (int c : tmpIn) inList.push_back(c);
                for (int c : tmpOut) outList.push_back(c);
                active.push_back(qi);
            }
        }
        inSt[qi + 1] = (int)inList.size(); outSt[qi + 1] = (int)outList.size();
    }
    vector<int>().swap(hk.pu); vector<int>().swap(hk.pv); vector<int>().swap(eu); vector<int>().swap(ev);

    vector<long long> ansSel(qn, 0), ansNot(qn, 0), addInSel(K, 0), addInNot(K, 0), addOutSel(K, 0), addOutNot(K, 0);

    const int BLOCK = 8192;
    const int W = (BLOCK + 63) >> 6;
    bool needBlocks = !oneIn.empty() || !oneOut.empty() || !active.empty();
    vector<ull> reach, pred;
    alignas(64) ull baseIB[W], baseOB[W], leftB[W], bufA[W], bufB[W];

    auto addByBits = [&](ull x, bool inSide, bool allL, bool allR, int w, long long &s, long long &ns) {
        if (!x) return;
        long long z = pc(x), l = allL ? z : (allR ? 0 : pc(x & leftB[w]));
        long long r = z - l;
        if (inSide) s += r, ns += l;
        else s += l, ns += r;
    };
    auto addSingle = [&](const ull *row, const ull *ex, bool inSide, bool allL, bool allR) {
        long long s = 0, ns = 0;
        for (int w = 0; w < W; ++w) addByBits(row[w] & ~ex[w], inSide, allL, allR, w, s, ns);
        return pair<long long,long long>(s, ns);
    };

    if (needBlocks) {
        const size_t MW = (size_t)K * W;
        reach.assign(MW, 0); pred.assign(MW, 0);
        for (int lo = 0; lo < V; lo += BLOCK) {
            int hi = min(V, lo + BLOCK);
            bool allL = hi <= n, allR = lo >= n;
            memset(reach.data(), 0, MW * sizeof(ull));
            memset(pred.data(), 0, MW * sizeof(ull));
            memset(baseIB, 0, sizeof(baseIB)); memset(baseOB, 0, sizeof(baseOB)); memset(leftB, 0, sizeof(leftB));
            for (int x = lo, b = 0; x < hi; ++x, ++b) {
                int c = comp[x], w = b >> 6; ull mask = 1ULL << (b & 63);
                reach[(size_t)c * W + w] |= mask; pred[(size_t)c * W + w] |= mask;
                if (baseIn[c]) baseIB[w] |= mask;
                if (baseOut[c]) baseOB[w] |= mask;
                if (x < n) leftB[w] |= mask;
            }
            for (int ti = K - 1; ti >= 0; --ti) {
                int u = topo[ti]; ull *ru = reach.data() + (size_t)u * W;
                for (int p = dag.st[u]; p < dag.st[u + 1]; ++p) orWords(ru, reach.data() + (size_t)dag.to[p] * W, W);
            }
            for (int ti = 0; ti < K; ++ti) {
                int u = topo[ti]; const ull *pu = pred.data() + (size_t)u * W;
                for (int p = dag.st[u]; p < dag.st[u + 1]; ++p) orWords(pred.data() + (size_t)dag.to[p] * W, pu, W);
            }
            for (int c : oneIn) {
                auto z = addSingle(reach.data() + (size_t)c * W, baseIB, true, allL, allR);
                addInSel[c] += z.first; addInNot[c] += z.second;
            }
            for (int c : oneOut) {
                auto z = addSingle(pred.data() + (size_t)c * W, baseOB, false, allL, allR);
                addOutSel[c] += z.first; addOutNot[c] += z.second;
            }
            for (int qi : active) {
                int is = inSt[qi], ie = inSt[qi + 1], os = outSt[qi], oe = outSt[qi + 1];
                bool hasI = is < ie, hasO = os < oe;
                bool conf = false;
                long long s = 0, ns = 0;
                int cond = (ie - is) + (oe - os);

                if (cond <= 4) {
                    if (hasI && !hasO) {
                        if (allL) {
                            for (int w = 0; w < W; ++w) {
                                ull a = 0;
                                for (int p = is; p < ie; ++p) a |= reach[(size_t)inList[p] * W + w];
                                ns += pc(a & ~baseIB[w]);
                            }
                        } else if (allR) {
                            for (int w = 0; w < W; ++w) {
                                ull a = 0;
                                for (int p = is; p < ie; ++p) a |= reach[(size_t)inList[p] * W + w];
                                s += pc(a & ~baseIB[w]);
                            }
                        } else {
                            for (int w = 0; w < W; ++w) {
                                ull a = 0;
                                for (int p = is; p < ie; ++p) a |= reach[(size_t)inList[p] * W + w];
                                a &= ~baseIB[w];
                                int tot = pc(a), lef = pc(a & leftB[w]);
                                s += tot - lef; ns += lef;
                            }
                        }
                    } else if (!hasI && hasO) {
                        if (allL) {
                            for (int w = 0; w < W; ++w) {
                                ull b = 0;
                                for (int p = os; p < oe; ++p) b |= pred[(size_t)outList[p] * W + w];
                                s += pc(b & ~baseOB[w]);
                            }
                        } else if (allR) {
                            for (int w = 0; w < W; ++w) {
                                ull b = 0;
                                for (int p = os; p < oe; ++p) b |= pred[(size_t)outList[p] * W + w];
                                ns += pc(b & ~baseOB[w]);
                            }
                        } else {
                            for (int w = 0; w < W; ++w) {
                                ull b = 0;
                                for (int p = os; p < oe; ++p) b |= pred[(size_t)outList[p] * W + w];
                                b &= ~baseOB[w];
                                int tot = pc(b), lef = pc(b & leftB[w]);
                                s += lef; ns += tot - lef;
                            }
                        }
                    } else {
                        if (allL) {
                            for (int w = 0; w < W; ++w) {
                                ull a = 0, b = 0;
                                for (int p = is; p < ie; ++p) a |= reach[(size_t)inList[p] * W + w];
                                for (int p = os; p < oe; ++p) b |= pred[(size_t)outList[p] * W + w];
                                a &= ~baseIB[w]; b &= ~baseOB[w];
                                if (a & b) conf = true;
                                ns += pc(a); s += pc(b);
                            }
                        } else if (allR) {
                            for (int w = 0; w < W; ++w) {
                                ull a = 0, b = 0;
                                for (int p = is; p < ie; ++p) a |= reach[(size_t)inList[p] * W + w];
                                for (int p = os; p < oe; ++p) b |= pred[(size_t)outList[p] * W + w];
                                a &= ~baseIB[w]; b &= ~baseOB[w];
                                if (a & b) conf = true;
                                s += pc(a); ns += pc(b);
                            }
                        } else {
                            for (int w = 0; w < W; ++w) {
                                ull a = 0, b = 0;
                                for (int p = is; p < ie; ++p) a |= reach[(size_t)inList[p] * W + w];
                                for (int p = os; p < oe; ++p) b |= pred[(size_t)outList[p] * W + w];
                                a &= ~baseIB[w]; b &= ~baseOB[w];
                                if (a & b) conf = true;
                                int at = pc(a), al = pc(a & leftB[w]);
                                int bt = pc(b), bl = pc(b & leftB[w]);
                                s += (at - al) + bl;
                                ns += al + (bt - bl);
                            }
                        }
                    }
                } else {
                    if (hasI) {
                        memset(bufA, 0, sizeof(bufA));
                        for (int p = is; p < ie; ++p) orWords(bufA, reach.data() + (size_t)inList[p] * W, W);
                        for (int w = 0; w < W; ++w) bufA[w] &= ~baseIB[w];
                    }
                    if (hasO) {
                        memset(bufB, 0, sizeof(bufB));
                        for (int p = os; p < oe; ++p) orWords(bufB, pred.data() + (size_t)outList[p] * W, W);
                        for (int w = 0; w < W; ++w) bufB[w] &= ~baseOB[w];
                    }
                    if (hasI && !hasO) {
                        if (allL) for (int w = 0; w < W; ++w) ns += pc(bufA[w]);
                        else if (allR) for (int w = 0; w < W; ++w) s += pc(bufA[w]);
                        else for (int w = 0; w < W; ++w) { int tot = pc(bufA[w]), lef = pc(bufA[w] & leftB[w]); s += tot - lef; ns += lef; }
                    } else if (!hasI && hasO) {
                        if (allL) for (int w = 0; w < W; ++w) s += pc(bufB[w]);
                        else if (allR) for (int w = 0; w < W; ++w) ns += pc(bufB[w]);
                        else for (int w = 0; w < W; ++w) { int tot = pc(bufB[w]), lef = pc(bufB[w] & leftB[w]); s += lef; ns += tot - lef; }
                    } else {
                        if (allL) {
                            for (int w = 0; w < W; ++w) { if (bufA[w] & bufB[w]) conf = true; ns += pc(bufA[w]); s += pc(bufB[w]); }
                        } else if (allR) {
                            for (int w = 0; w < W; ++w) { if (bufA[w] & bufB[w]) conf = true; s += pc(bufA[w]); ns += pc(bufB[w]); }
                        } else {
                            for (int w = 0; w < W; ++w) {
                                if (bufA[w] & bufB[w]) conf = true;
                                int at = pc(bufA[w]), al = pc(bufA[w] & leftB[w]);
                                int bt = pc(bufB[w]), bl = pc(bufB[w] & leftB[w]);
                                s += (at - al) + bl;
                                ns += al + (bt - bl);
                            }
                        }
                    }
                }
                if (conf) bad[qi] = 1;
                else ansSel[qi] += s, ansNot[qi] += ns;
            }
            if (!active.empty()) {
                int w = 0;
                for (int qi : active) if (!bad[qi]) active[w++] = qi;
                active.resize(w);
            }
        }
    }

    string out;
    out.reserve((size_t)qn * 24);
    for (int i = 0; i < qn; ++i) {
        if (bad[i]) out += "-1\n";
        else if (type[i] == 1) out += to_string(baseSel) + " " + to_string(baseNot) + "\n";
        else if (type[i] == 2) { int c = oneComp[i]; out += to_string(baseSel + addInSel[c]) + " " + to_string(baseNot + addInNot[c]) + "\n"; }
        else if (type[i] == 3) { int c = oneComp[i]; out += to_string(baseSel + addOutSel[c]) + " " + to_string(baseNot + addOutNot[c]) + "\n"; }
        else out += to_string(baseSel + ansSel[i]) + " " + to_string(baseNot + ansNot[i]) + "\n";
    }
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
