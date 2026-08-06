#include <cmath>
#include <queue>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

const int N = 100000 + 5;
const int M = 262144 + 5;
const int D = 650000 + 5;

const int MAXT = 99999;
const long double angle_dis = 2 * acosl(-1.0) / (MAXT + 1);

int n, rs, ts, rt, tt, cnt;
int Max[M], Tag[M];

int head[D], edge_cnt;
struct Edge {
    int to, next;
    long double w;
} edge[D * 4];

inline void add_edge(int u, int v, long double w) {
    edge[++edge_cnt] = {v, head[u], w};
    head[u] = edge_cnt;
}

vector<int> Angles[N], Arcs[N]; 
long double Dist[D];
bool vis[D];

struct Arc {
    int r, t1, t2;
    bool In(int t) const {
        if (t1 <= t2) return t1 <= t && t <= t2;
        return t >= t1 || t <= t2;
    }
    bool operator < (const Arc &rhs) const { return r > rhs.r; }
} A[N];

struct Point {
    int r, t, arc_idx;
    bool operator < (const Point &rhs) const {
        if (r != rhs.r) return r < rhs.r;
        return t < rhs.t;
    }
    bool operator == (const Point &rhs) const {
        return r == rhs.r && t == rhs.t;
    }
};
vector<Point> tmp_pts;

#define ls(x) (x << 1)
#define rs(x) (x << 1 | 1)

void push(int x) {
    if (Tag[x]) {
        Max[ls(x)] = max(Max[ls(x)], Tag[x]);
        Max[rs(x)] = max(Max[rs(x)], Tag[x]);
        Tag[ls(x)] = max(Tag[ls(x)], Tag[x]);
        Tag[rs(x)] = max(Tag[rs(x)], Tag[x]);
        Tag[x] = 0;
    }
}

void Modify(int x, int l, int r, int ql, int qr, int v) {
    if (ql <= l && r <= qr) {
        Max[x] = max(Max[x], v);
        Tag[x] = max(Tag[x], v);
        return;
    }
    push(x);
    int mid = (l + r) >> 1;
    if (ql <= mid) Modify(ls(x), l, mid, ql, qr, v);
    if (qr > mid) Modify(rs(x), mid + 1, r, ql, qr, v);
}

int Query(int x, int l, int r, int pos) {
    if (l == r) return Max[x];
    push(x);
    int mid = (l + r) >> 1;
    if (pos <= mid) return Query(ls(x), l, mid, pos);
    return Query(rs(x), mid + 1, r, pos);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n >> rs >> ts >> rt >> tt)) return 0;
    for (int i = 1; i <= n; i++) {
        cin >> A[i].r >> A[i].t1 >> A[i].t2;
    }
    
    sort(A + 1, A + n + 1);
    
    for (int i = 1; i <= n; i++) {
        tmp_pts.push_back({A[i].r, A[i].t1, i});
        tmp_pts.push_back({A[i].r, A[i].t2, i});
        if (A[i].In(ts)) tmp_pts.push_back({A[i].r, ts, i});
        if (A[i].In(tt)) tmp_pts.push_back({A[i].r, tt, i});
        
        int lid = Query(1, 0, MAXT, A[i].t1);
        int rid = Query(1, 0, MAXT, A[i].t2);
        if (lid != 0) tmp_pts.push_back({A[lid].r, A[i].t1, lid});
        if (rid != 0) tmp_pts.push_back({A[rid].r, A[i].t2, rid});
        
        if (A[i].t1 <= A[i].t2) Modify(1, 0, MAXT, A[i].t1, A[i].t2, i);
        else {
            Modify(1, 0, MAXT, A[i].t1, MAXT, i);
            Modify(1, 0, MAXT, 0, A[i].t2, i);
        }
    }

    sort(tmp_pts.begin(), tmp_pts.end());
    tmp_pts.erase(unique(tmp_pts.begin(), tmp_pts.end()), tmp_pts.end());
    
    cnt = tmp_pts.size();
    
    int St = 0, Ed = 0;
    for (int i = 0; i < cnt; i++) {
        Angles[tmp_pts[i].t].push_back(i);
        Arcs[tmp_pts[i].arc_idx].push_back(i);
        if (tmp_pts[i].r == rs && tmp_pts[i].t == ts) St = i + 1;
        if (tmp_pts[i].r == rt && tmp_pts[i].t == tt) Ed = i + 1;
    }

    for (int t = 0; t <= MAXT; t++) {
        if (Angles[t].size() < 2) continue;
        for (size_t i = 1; i < Angles[t].size(); i++) {
            int u = Angles[t][i], v = Angles[t][i - 1];
            long double w = (long double)(tmp_pts[u].r - tmp_pts[v].r);
            add_edge(u + 1, v + 1, w);
            add_edge(v + 1, u + 1, w);
        }
    }

    for (int i = 1; i <= n; i++) {
        if (Arcs[i].size() < 2) continue;
        sort(Arcs[i].begin(), Arcs[i].end(), [&](int idx1, int idx2) {
            int t1 = tmp_pts[idx1].t, t2 = tmp_pts[idx2].t;
            int d1 = t1 >= A[i].t1 ? t1 - A[i].t1 : t1 + MAXT + 1 - A[i].t1;
            int d2 = t2 >= A[i].t1 ? t2 - A[i].t1 : t2 + MAXT + 1 - A[i].t1;
            return d1 < d2;
        });
        for (size_t j = 1; j < Arcs[i].size(); j++) {
            int u = Arcs[i][j], v = Arcs[i][j - 1];
            int t1 = tmp_pts[u].t, t2 = tmp_pts[v].t;
            long double w = (long double)(t1 >= t2 ? t1 - t2 : t1 + MAXT + 1 - t2) * angle_dis * A[i].r;
            add_edge(u + 1, v + 1, w);
            add_edge(v + 1, u + 1, w);
        }
    }

    fill(Dist + 1, Dist + cnt + 1, 1e18);
    priority_queue<pair<long double, int>, vector<pair<long double, int>>, greater<pair<long double, int>>> Q;
    
    Dist[St] = 0;
    Q.push({0, St});
    
    while (!Q.empty()) {
        auto [d, u] = Q.top();
        Q.pop();
        if (vis[u]) continue; 
        vis[u] = true;
        if (u == Ed) break;
        
        for (int e = head[u]; e; e = edge[e].next) {
            int v = edge[e].to;
            if (!vis[v] && Dist[v] > d + edge[e].w) {
                Dist[v] = d + edge[e].w;
                Q.push({Dist[v], v});
            }
        }
    }
    
    if (Dist[Ed] > 1e17) cout << "Cast off as cast!\n";
    else printf("%.12Lf\n", Dist[Ed]);
        
    return 0;
}

