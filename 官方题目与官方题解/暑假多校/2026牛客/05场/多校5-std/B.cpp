#include <bits/stdc++.h>
using namespace std;

using int64 = long long;
using i128 = __int128;

const int64 MOD = 998244353;
const int64 INV2 = (MOD + 1) / 2;

struct Point {
    int64 x, y;

    bool operator < (const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }

    bool operator == (const Point& other) const {
        return x == other.x && y == other.y;
    }
};

Point operator + (const Point& a, const Point& b) {
    return {a.x + b.x, a.y + b.y};
}

Point operator - (const Point& a, const Point& b) {
    return {a.x - b.x, a.y - b.y};
}

i128 crossVec(const Point& a, const Point& b) {
    return (i128)a.x * b.y - (i128)a.y * b.x;
}

i128 cross(const Point& o, const Point& a, const Point& b) {
    return crossVec(a - o, b - o);
}

vector<Point> convexHull(vector<Point> p) {
    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end()), p.end());

    if ((int)p.size() <= 1) return p;

    vector<Point> lower, upper;

    for (const Point& pt : p) {
        while ((int)lower.size() >= 2 && cross(lower[(int)lower.size() - 2], lower.back(), pt) <= 0) {
            lower.pop_back();
        }
        lower.push_back(pt);
    }

    for (int i = (int)p.size() - 1; i >= 0; --i) {
        const Point& pt = p[i];
        while ((int)upper.size() >= 2 && cross(upper[(int)upper.size() - 2], upper.back(), pt) <= 0) {
            upper.pop_back();
        }
        upper.push_back(pt);
    }

    lower.pop_back();
    upper.pop_back();

    vector<Point> h = lower;
    h.insert(h.end(), upper.begin(), upper.end());

    return h;
}

i128 signedArea2(const vector<Point>& p) {
    i128 s = 0;
    int n = (int)p.size();
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        s += (i128)p[i].x * p[j].y - (i128)p[j].x * p[i].y;
    }
    return s;
}

i128 area2(const vector<Point>& p) {
    i128 s = signedArea2(p);
    return s >= 0 ? s : -s;
}

int64 boundaryLatticePoints(const vector<Point>& p) {
    int64 res = 0;
    int n = (int)p.size();
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        int64 dx = llabs(p[i].x - p[j].x);
        int64 dy = llabs(p[i].y - p[j].y);
        res += std::gcd(dx, dy);
    }
    return res;
}

vector<Point> rotateToLowestLeft(vector<Point> p) {
    int id = 0;
    for (int i = 1; i < (int)p.size(); ++i) {
        if (make_pair(p[i].y, p[i].x) < make_pair(p[id].y, p[id].x)) {
            id = i;
        }
    }
    rotate(p.begin(), p.begin() + id, p.end());
    return p;
}

vector<Point> minkowskiSum(vector<Point> a, vector<Point> b) {
    a = rotateToLowestLeft(a);
    b = rotateToLowestLeft(b);

    int n = (int)a.size();
    int m = (int)b.size();

    vector<Point> ea(n), eb(m);
    for (int i = 0; i < n; ++i) ea[i] = a[(i + 1) % n] - a[i];
    for (int i = 0; i < m; ++i) eb[i] = b[(i + 1) % m] - b[i];

    vector<Point> res;
    res.reserve(n + m);

    Point cur = a[0] + b[0];
    res.push_back(cur);

    int i = 0, j = 0;

    while (i < n || j < m) {
        Point step;

        if (j == m) {
            step = ea[i++];
        } else if (i == n) {
            step = eb[j++];
        } else {
            i128 cr = crossVec(ea[i], eb[j]);

            if (cr > 0) {
                step = ea[i++];
            } else if (cr < 0) {
                step = eb[j++];
            } else {
                step = ea[i] + eb[j];
                ++i;
                ++j;
            }
        }

        cur = cur + step;
        res.push_back(cur);
    }

    res.pop_back();
    return res;
}

int64 normMod(i128 x) {
    x %= MOD;
    if (x < 0) x += MOD;
    return (int64)x;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    cin >> n >> m >> q;

    vector<Point> A(n), B(m);

    for (auto& p : A) cin >> p.x >> p.y;
    for (auto& p : B) cin >> p.x >> p.y;

    vector<Point> P = convexHull(A);
    vector<Point> Q = convexHull(B);

    vector<Point> negP = P;
    for (Point& p : negP) {
        p.x = -p.x;
        p.y = -p.y;
    }

    i128 SP = area2(P);
    i128 SQ = area2(Q);

    int64 BP = boundaryLatticePoints(P);
    int64 BQ = boundaryLatticePoints(Q);

    vector<Point> QR = minkowskiSum(Q, negP);

    i128 SQR = area2(QR);
    i128 C = SQR - SQ - SP;

    int64 sp = normMod(SP);
    int64 sq = normMod(SQ);
    int64 bp = normMod(BP);
    int64 bq = normMod(BQ);
    int64 c = normMod(C);

    while (q--) {
        int64 kInput;
        cin >> kInput;

        int64 k = kInput % MOD;
        int64 k2 = k * k % MOD;

        int64 ans = 0;

        ans = (ans + sq) % MOD;
        ans = (ans + k2 * sp) % MOD;
        ans = (ans + k * c) % MOD;
        ans = (ans + bq) % MOD;
        ans = (ans + k * bp) % MOD;
        ans = (ans + 2) % MOD;

        ans = ans * INV2 % MOD;

        cout << ans << '\n';
    }

    return 0;
}
