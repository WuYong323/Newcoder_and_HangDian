#include <bits/stdc++.h>
using namespace std;
 
using ld = long double;
 
const ld EPS = 1e-12L;
const ld INF = 1e100L;
 
struct Point {
    ld x, y;
 
    Point(ld _x = 0, ld _y = 0) : x(_x), y(_y) {}
 
    Point operator + (const Point& o) const {
        return Point(x + o.x, y + o.y);
    }
 
    Point operator - (const Point& o) const {
        return Point(x - o.x, y - o.y);
    }
 
    Point operator * (ld k) const {
        return Point(x * k, y * k);
    }
};
 
ld dot(Point a, Point b) {
    return a.x * b.x + a.y * b.y;
}
 
ld cross(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}
 
ld norm2(Point a) {
    return dot(a, a);
}
 
ld norm(Point a) {
    return sqrtl(norm2(a));
}
 
struct Edge {
    // Polygon is represented as u · x <= h.
    // u is the outward unit normal.
    Point u;
    ld h;
};
 
struct WeightedPoint {
    Point p;
    long long w;
};
 
struct Segment {
    Point a, b;
    ld ra, rb;
};
 
struct HalfPlane {
    // Boundary line: p + v * t.
    // Inside is the left side of v:
    // cross(v, x - p) >= 0.
    Point p, v;
    ld ang;
 
    // type = 0: original polygon edge
    // type = 1: Voronoi bisector d_i = d_id
    int type;
    int id;
};
 
struct ScanEvent {
    ld x;
    int type; // 0 = add, 1 = remove
    long long w;
};
 
HalfPlane makeHalfPlane(ld a, ld b, ld c, int type, int id) {
    // Inequality: a x + b y <= c.
    //
    // Let v = (-b, a), and choose p on the line.
    // Then cross(v, q - p) = c - a q.x - b q.y.
    // So cross(v, q - p) >= 0 is exactly a q.x + b q.y <= c.
    ld den = a * a + b * b;
 
    Point p(a * c / den, b * c / den);
    Point v(-b, a);
 
    return HalfPlane{p, v, atan2l(v.y, v.x), type, id};
}
 
bool inside(const HalfPlane& hp, Point q) {
    return cross(hp.v, q - hp.p) >= -EPS;
}
 
bool parallel(const HalfPlane& a, const HalfPlane& b) {
    return fabsl(cross(a.v, b.v)) <= 1e-18L;
}
 
Point lineIntersection(const HalfPlane& a, const HalfPlane& b) {
    ld d = cross(a.v, b.v);
 
    if (fabsl(d) <= 1e-30L) {
        return Point(INF, INF);
    }
 
    ld t = cross(b.p - a.p, b.v) / d;
 
    return a.p + a.v * t;
}
 
vector<HalfPlane> halfPlaneIntersection(vector<HalfPlane> hp) {
    sort(hp.begin(), hp.end(), [](const HalfPlane& a, const HalfPlane& b) {
        if (fabsl(a.ang - b.ang) > 1e-18L) {
            return a.ang < b.ang;
        }
 
        return cross(a.v, b.p - a.p) < 0;
    });
 
    vector<HalfPlane> lines;
    lines.reserve(hp.size());
 
    for (auto h : hp) {
        if (lines.empty() || fabsl(h.ang - lines.back().ang) > 1e-18L) {
            lines.push_back(h);
        } else {
            // Same direction. Keep the more restrictive half-plane.
            if (inside(lines.back(), h.p)) {
                lines.back() = h;
            }
        }
    }
 
    deque<HalfPlane> q;
 
    for (auto h : lines) {
        while (q.size() >= 2) {
            Point p = lineIntersection(q[q.size() - 2], q[q.size() - 1]);
            if (inside(h, p)) break;
            q.pop_back();
        }
 
        while (q.size() >= 2) {
            Point p = lineIntersection(q[0], q[1]);
            if (inside(h, p)) break;
            q.pop_front();
        }
 
        q.push_back(h);
    }
 
    while (q.size() >= 3) {
        Point p = lineIntersection(q[q.size() - 2], q[q.size() - 1]);
        if (inside(q[0], p)) break;
        q.pop_back();
    }
 
    while (q.size() >= 3) {
        Point p = lineIntersection(q[0], q[1]);
        if (inside(q.back(), p)) break;
        q.pop_front();
    }
 
    if (q.size() < 3) {
        return {};
    }
 
    return vector<HalfPlane>(q.begin(), q.end());
}
 
vector<Point> simplifyPolygon(vector<Point> p) {
    int n = (int)p.size();
 
    ld area = 0;
 
    for (int i = 0; i < n; i++) {
        area += cross(p[i], p[(i + 1) % n]);
    }
 
    if (area < 0) {
        reverse(p.begin(), p.end());
    }
 
    vector<Point> q;
 
    for (auto v : p) {
        if (q.empty() || norm2(v - q.back()) > 1e-24L) {
            q.push_back(v);
        }
    }
 
    if (q.size() > 1 && norm2(q.front() - q.back()) <= 1e-24L) {
        q.pop_back();
    }
 
    bool changed = true;
 
    while (changed && q.size() >= 3) {
        changed = false;
 
        vector<Point> r;
        int m = (int)q.size();
 
        for (int i = 0; i < m; i++) {
            Point a = q[(i - 1 + m) % m];
            Point b = q[i];
            Point c = q[(i + 1) % m];
 
            Point u = b - a;
            Point v = c - b;
 
            if (fabsl(cross(u, v)) <= 1e-18L && dot(u, v) >= -1e-18L) {
                changed = true;
                continue;
            }
 
            r.push_back(b);
        }
 
        q.swap(r);
    }
 
    return q;
}
 
vector<Edge> buildEdges(const vector<Point>& poly) {
    int n = (int)poly.size();
 
    vector<Edge> edges(n);
 
    for (int i = 0; i < n; i++) {
        Point p = poly[i];
        Point q = poly[(i + 1) % n];
        Point e = q - p;
 
        ld len = norm(e);
 
        // For a CCW polygon, outward normal is (e.y, -e.x).
        Point u(e.y / len, -e.x / len);
        ld h = dot(u, p);
 
        edges[i] = {u, h};
    }
 
    return edges;
}
 
vector<Segment> buildVoronoiSegments(const vector<Edge>& edges) {
    int n = (int)edges.size();
 
    vector<HalfPlane> base;
    base.reserve(n);
 
    for (int i = 0; i < n; i++) {
        base.push_back(makeHalfPlane(
            edges[i].u.x,
            edges[i].u.y,
            edges[i].h,
            0,
            i
        ));
    }
 
    vector<Segment> segs;
    segs.reserve(2 * n + 5);
 
    vector<HalfPlane> hp;
    hp.reserve(2 * n);
 
    for (int i = 0; i < n; i++) {
        hp.clear();
 
        for (auto h : base) {
            hp.push_back(h);
        }
 
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
 
            // d_i(x) <= d_j(x)
            //
            // d_i(x) = h_i - u_i · x
            // d_j(x) = h_j - u_j · x
            //
            // h_i - u_i · x <= h_j - u_j · x
            // (u_j - u_i) · x <= h_j - h_i
            ld a = edges[j].u.x - edges[i].u.x;
            ld b = edges[j].u.y - edges[i].u.y;
            ld c = edges[j].h - edges[i].h;
 
            if (a * a + b * b <= 1e-30L) {
                continue;
            }
 
            hp.push_back(makeHalfPlane(a, b, c, 1, j));
        }
 
        vector<HalfPlane> cell = halfPlaneIntersection(hp);
 
        int s = (int)cell.size();
 
        if (s < 3) continue;
 
        vector<Point> vertex(s);
        bool ok = true;
 
        for (int k = 0; k < s; k++) {
            int nxt = (k + 1) % s;
 
            if (parallel(cell[k], cell[nxt])) {
                ok = false;
                break;
            }
 
            vertex[k] = lineIntersection(cell[k], cell[nxt]);
 
            if (!isfinite((double)vertex[k].x) || !isfinite((double)vertex[k].y)) {
                ok = false;
                break;
            }
        }
 
        if (!ok) continue;
 
        for (int k = 0; k < s; k++) {
            if (cell[k].type != 1) continue;
 
            int j = cell[k].id;
 
            // The same Voronoi edge appears in cell i and cell j.
            // Keep only one copy.
            if (i > j) continue;
 
            Point A = vertex[(k - 1 + s) % s];
            Point B = vertex[k];
 
            if (norm2(A - B) <= 1e-24L) continue;
 
            ld ra = edges[i].h - dot(edges[i].u, A);
            ld rb = edges[i].h - dot(edges[i].u, B);
 
            if (ra < -1e-7L || rb < -1e-7L) {
                continue;
            }
 
            ra = max((ld)0, ra);
            rb = max((ld)0, rb);
 
            segs.push_back({A, B, ra, rb});
        }
    }
 
    return segs;
}
 
bool addInterval(vector<ScanEvent>& ev, ld l, ld r, long long w) {
    if (r < -EPS || l > 1 + EPS) {
        return false;
    }
 
    l = max((ld)0, min((ld)1, l));
    r = max((ld)0, min((ld)1, r));
 
    if (l <= r + EPS) {
        ev.push_back({l, 0, w});
        ev.push_back({r, 1, w});
        return true;
    }
 
    return false;
}
 
bool addCoverIntervals(
    const Segment& sg,
    const WeightedPoint& wp,
    ld lambda,
    vector<ScanEvent>& ev
) {
    Point p0 = sg.a * lambda;
    Point v = (sg.b - sg.a) * lambda;
    Point d = p0 - wp.p;
 
    ld r0 = sg.ra * lambda;
    ld u = (sg.rb - sg.ra) * lambda;
 
    // Need |p0 + v t - wp.p|^2 <= (r0 + u t)^2.
    //
    // A t^2 + B t + C <= 0.
    ld A = dot(v, v) - u * u;
    ld B = 2 * (dot(d, v) - r0 * u);
    ld C = dot(d, d) - r0 * r0;
 
    ld scale = max((ld)1, max(fabsl(A), max(fabsl(B), fabsl(C))));
    ld eps = 1e-18L * scale;
 
    bool any = false;
 
    if (fabsl(A) <= eps) {
        if (fabsl(B) <= eps) {
            if (C <= eps) {
                any |= addInterval(ev, 0, 1, wp.w);
            }
 
            return any;
        }
 
        ld root = -C / B;
 
        if (B > 0) {
            any |= addInterval(ev, 0, root, wp.w);
        } else {
            any |= addInterval(ev, root, 1, wp.w);
        }
 
        return any;
    }
 
    ld D = B * B - 4 * A * C;
    ld dscale = max((ld)1, max(fabsl(B * B), fabsl(4 * A * C)));
    ld deps = 1e-18L * dscale;
 
    if (A > 0) {
        if (D < -deps) {
            return false;
        }
 
        if (D < 0) D = 0;
 
        ld sq = sqrtl(D);
        ld l = (-B - sq) / (2 * A);
        ld r = (-B + sq) / (2 * A);
 
        if (l > r) swap(l, r);
 
        any |= addInterval(ev, l, r, wp.w);
    } else {
        // This branch should almost never be used for valid medial-axis
        // segments, but keeps the solver numerically safe.
        if (D < -deps) {
            any |= addInterval(ev, 0, 1, wp.w);
            return any;
        }
 
        if (D < 0) D = 0;
 
        ld sq = sqrtl(D);
        ld l = (-B - sq) / (2 * A);
        ld r = (-B + sq) / (2 * A);
 
        if (l > r) swap(l, r);
 
        any |= addInterval(ev, 0, l, wp.w);
        any |= addInterval(ev, r, 1, wp.w);
    }
 
    return any;
}
 
bool feasibleOriginDisk(
    ld lambda,
    const vector<WeightedPoint>& pts,
    long long W,
    ld inRad
) {
    ld R = lambda * inRad;
    ld R2 = R * R;
    ld eps = 1e-16L * max((ld)1, R2);
 
    long long sum = 0;
 
    for (const auto& pt : pts) {
        if (norm2(pt.p) <= R2 + eps) {
            sum += pt.w;
 
            if (sum >= W) {
                return true;
            }
        }
    }
 
    return false;
}
 
bool feasible(
    ld lambda,
    const vector<Segment>& segs,
    const vector<WeightedPoint>& pts,
    long long W,
    ld inRad
) {
    if (feasibleOriginDisk(lambda, pts, W, inRad)) {
        return true;
    }
 
    vector<ScanEvent> ev;
    ev.reserve(2 * pts.size() + 5);
 
    for (const auto& sg : segs) {
        ev.clear();
 
        long long potential = 0;
 
        for (const auto& p : pts) {
            bool any = addCoverIntervals(sg, p, lambda, ev);
 
            if (any) {
                potential += p.w;
            }
        }
 
        if (potential < W || ev.empty()) {
            continue;
        }
 
        sort(ev.begin(), ev.end(), [](const ScanEvent& a, const ScanEvent& b) {
            if (a.x != b.x) {
                return a.x < b.x;
            }
 
            return a.type < b.type;
        });
 
        long long cur = 0;
 
        for (int i = 0; i < (int)ev.size(); ) {
            ld x = ev[i].x;
 
            long long add = 0;
            long long sub = 0;
 
            int j = i;
 
            while (j < (int)ev.size() && fabsl(ev[j].x - x) <= 1e-14L) {
                if (ev[j].type == 0) {
                    add += ev[j].w;
                } else {
                    sub += ev[j].w;
                }
 
                j++;
            }
 
            // Closed intervals: additions are counted before removals.
            cur += add;
 
            if (cur >= W) {
                return true;
            }
 
            cur -= sub;
            i = j;
        }
    }
 
    return false;
}
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int T;
    cin >> T;
 
    cout.setf(ios::fixed);
    cout << setprecision(10);
 
    while (T--) {
        int n, m;
        long long W;
 
        cin >> n >> m >> W;
 
        vector<Point> poly(n);
 
        for (int i = 0; i < n; i++) {
            long long x, y;
            cin >> x >> y;
 
            poly[i] = Point((ld)x, (ld)y);
        }
 
        vector<WeightedPoint> pts(m);
 
        for (int i = 0; i < m; i++) {
            long long x, y, a;
            cin >> x >> y >> a;
 
            pts[i] = {Point((ld)x, (ld)y), a};
        }
 
        poly = simplifyPolygon(poly);
        vector<Edge> edges = buildEdges(poly);
 
        ld inRad = INF;
 
        for (auto e : edges) {
            inRad = min(inRad, e.h);
        }
 
        inRad = max(inRad, (ld)1e-18L);
 
        vector<Segment> segs = buildVoronoiSegments(edges);
 
        if (feasible(0, segs, pts, W, inRad)) {
            cout << 0.0 << '\n';
            continue;
        }
 
        ld maxNorm = 0;
 
        for (auto pt : pts) {
            maxNorm = max(maxNorm, norm(pt.p));
        }
 
        ld lo = 0;
        ld hi = max((ld)1, maxNorm / inRad);
 
        // This upper bound should be feasible because the origin-centered disk
        // with radius maxNorm covers all points and lies inside hi * A.
        if (!feasible(hi, segs, pts, W, inRad)) {
            for (int rep = 0; rep < 20; rep++) {
                hi *= 2;
 
                if (feasible(hi, segs, pts, W, inRad)) {
                    break;
                }
            }
        }
 
        for (int it = 0; (hi - lo) / max(1.0L , lo) > 1e-7; it++) {
            ld mid = (lo + hi) / 2;
 
            if (feasible(mid, segs, pts, W, inRad)) {
                hi = mid;
            } else {
                lo = mid;
            }
        }
 
        cout << (double)hi << '\n';
    }
 
    return 0;
}
