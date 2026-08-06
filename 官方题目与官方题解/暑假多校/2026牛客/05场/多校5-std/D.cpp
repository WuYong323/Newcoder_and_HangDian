// std.cpp - exact LP basis simplex, O(n^2 + pivots * n)
#include <bits/stdc++.h>
using namespace std;
using i128 = __int128_t;

struct HP {
    long long a, b, c;
};

vector<HP> h;

struct Vec3 {
    long long x, y, z;
};

static inline i128 crossVec(long long ax, long long ay, long long bx, long long by) {
    return (i128)ax * by - (i128)ay * bx;
}

static inline i128 crossHP(int i, int j) {
    return crossVec(h[i].a, h[i].b, h[j].a, h[j].b);
}

static inline i128 dotHP(int i, int j) {
    return (i128)h[i].a * h[j].a + (i128)h[i].b * h[j].b;
}

static inline Vec3 col(int idx) {
    return {h[idx].a, h[idx].b, 1};
}

static inline i128 det3(const Vec3& A, const Vec3& B, const Vec3& C) {
    return (i128)A.x * ((i128)B.y * C.z - (i128)B.z * C.y)
         - (i128)A.y * ((i128)B.x * C.z - (i128)B.z * C.x)
         + (i128)A.z * ((i128)B.x * C.y - (i128)B.y * C.x);
}

bool coverPair(int i, int j) {
    if (crossHP(i, j) != 0) return false;
    if (dotHP(i, j) >= 0) return false;

    long long vi = (h[i].a != 0 ? h[i].a : h[i].b);
    long long vj = (h[i].a != 0 ? h[j].a : h[j].b);

    i128 A = -(i128)vj;
    i128 B = (i128)vi;
    if (B < 0) {
        A = -A;
        B = -B;
    }

    return A * (i128)h[i].c + B * (i128)h[j].c >= 0;
}

bool coverTriple(int i, int j, int k) {
    if (coverPair(i, j) || coverPair(i, k) || coverPair(j, k)) return true;

    i128 w1 = crossHP(j, k);
    i128 w2 = crossHP(k, i);
    i128 w3 = crossHP(i, j);

    bool nonneg = (w1 >= 0 && w2 >= 0 && w3 >= 0);
    bool nonpos = (w1 <= 0 && w2 <= 0 && w3 <= 0);

    if (!nonneg && !nonpos) return false;
    if (w1 == 0 && w2 == 0 && w3 == 0) return false;

    if (nonpos) {
        w1 = -w1;
        w2 = -w2;
        w3 = -w3;
    }

    return w1 * (i128)h[i].c + w2 * (i128)h[j].c + w3 * (i128)h[k].c >= 0;
}

struct Pt {
    long long x, y;
    int id;
};

static inline i128 cross3pt(const Pt& o, const Pt& a, const Pt& b) {
    return crossVec(a.x - o.x, a.y - o.y, b.x - o.x, b.y - o.y);
}

bool originInTri(const Pt& A, const Pt& B, const Pt& C) {
    i128 area = cross3pt(A, B, C);
    if (area == 0) return false;

    i128 s1 = crossVec(B.x - A.x, B.y - A.y, -A.x, -A.y);
    i128 s2 = crossVec(C.x - B.x, C.y - B.y, -B.x, -B.y);
    i128 s3 = crossVec(A.x - C.x, A.y - C.y, -C.x, -C.y);

    if (area > 0) return s1 >= 0 && s2 >= 0 && s3 >= 0;
    return s1 <= 0 && s2 <= 0 && s3 <= 0;
}

bool findInitialBasis(array<int, 3>& basis) {
    vector<Pt> pts;
    pts.reserve(h.size());

    for (int i = 0; i < (int)h.size(); ++i) {
        pts.push_back({h[i].a, h[i].b, i});
    }

    sort(pts.begin(), pts.end(), [](const Pt& p, const Pt& q) {
        if (p.x != q.x) return p.x < q.x;
        return p.y < q.y;
    });

    vector<Pt> u;
    for (auto& p : pts) {
        if (u.empty() || u.back().x != p.x || u.back().y != p.y) {
            u.push_back(p);
        }
    }

    if ((int)u.size() < 3) return false;

    vector<Pt> lower, upper;

    for (auto& p : u) {
        while (lower.size() >= 2 &&
               cross3pt(lower[lower.size() - 2], lower.back(), p) <= 0) {
            lower.pop_back();
        }
        lower.push_back(p);
    }

    for (int i = (int)u.size() - 1; i >= 0; --i) {
        Pt p = u[i];
        while (upper.size() >= 2 &&
               cross3pt(upper[upper.size() - 2], upper.back(), p) <= 0) {
            upper.pop_back();
        }
        upper.push_back(p);
    }

    vector<Pt> hull = lower;
    for (int i = 1; i + 1 < (int)upper.size(); ++i) {
        hull.push_back(upper[i]);
    }

    int m = hull.size();
    if (m < 3) return false;

    i128 area2 = 0;
    for (int i = 0; i < m; ++i) {
        Pt p = hull[i], q = hull[(i + 1) % m];
        area2 += crossVec(p.x, p.y, q.x, q.y);
    }

    if (area2 < 0) reverse(hull.begin(), hull.end());

    for (int i = 0; i < m; ++i) {
        Pt p = hull[i], q = hull[(i + 1) % m];
        i128 cr = crossVec(q.x - p.x, q.y - p.y, -p.x, -p.y);
        if (cr <= 0) return false;
    }

    Pt root = hull[0];

    for (int i = 1; i + 1 < m; ++i) {
        if (originInTri(root, hull[i], hull[i + 1])) {
            basis = {root.id, hull[i].id, hull[i + 1].id};
            return true;
        }
    }

    return false;
}

int cmpFraction(i128 a, i128 b, i128 c, i128 d) {
    unsigned long long x = (unsigned long long)a;
    unsigned long long y = (unsigned long long)b;
    unsigned long long z = (unsigned long long)c;
    unsigned long long w = (unsigned long long)d;
    bool rev = false;

    while (true) {
        unsigned long long q1 = x / y;
        unsigned long long q2 = z / w;

        if (q1 != q2) {
            int res = (q1 < q2 ? -1 : 1);
            return rev ? -res : res;
        }

        x %= y;
        z %= w;

        if (x == 0 || z == 0) {
            int res = 0;
            if (x != z) res = (x == 0 ? -1 : 1);
            return rev ? -res : res;
        }

        swap(x, y);
        swap(z, w);
        rev = !rev;
    }
}

struct Simplex3 {
    int n;
    array<int, 3> B;
    vector<char> inB;

    Vec3 rhs() const {
        return {0, 0, 1};
    }

    i128 D() const {
        return det3(col(B[0]), col(B[1]), col(B[2]));
    }

    i128 replacedDet(int pos, const Vec3& v) const {
        Vec3 c0 = col(B[0]);
        Vec3 c1 = col(B[1]);
        Vec3 c2 = col(B[2]);

        if (pos == 0) c0 = v;
        if (pos == 1) c1 = v;
        if (pos == 2) c2 = v;

        return det3(c0, c1, c2);
    }

    array<i128, 3> xNumerators() const {
        Vec3 r = rhs();
        return {replacedDet(0, r), replacedDet(1, r), replacedDet(2, r)};
    }

    array<i128, 3> directionNumerators(int entering) const {
        Vec3 v = col(entering);
        return {replacedDet(0, v), replacedDet(1, v), replacedDet(2, v)};
    }

    i128 normalizedReduced(int entering) const {
        i128 den = D();
        i128 sgn = (den > 0 ? 1 : -1);

        auto dnum = directionNumerators(entering);

        i128 num = (i128)h[entering].c * den;
        for (int p = 0; p < 3; ++p) {
            num -= (i128)h[B[p]].c * dnum[p];
        }

        return num * sgn;
    }

    i128 normalizedObjective() const {
        i128 den = D();
        i128 sgn = (den > 0 ? 1 : -1);

        auto xnum = xNumerators();

        i128 num = 0;
        for (int p = 0; p < 3; ++p) {
            num += (i128)h[B[p]].c * xnum[p];
        }

        return num * sgn;
    }

    void solve() {
        inB.assign(n, 0);
        for (int p = 0; p < 3; ++p) {
            inB[B[p]] = 1;
        }

        int pivots = 0;
        array<int, 3> bestB = B;
        i128 bestObj = normalizedObjective();
        while (true) {
            i128 curObj = normalizedObjective();
            if (curObj > bestObj) {
                bestObj = curObj;
                bestB = B;
            }
            if (++pivots > 200 * n + 10000) {
                B = bestB;
                return;
            }
            int enter = -1;

            for (int j = 0; j < n; ++j) {
                if (!inB[j] && normalizedReduced(j) > 0) {
                    enter = j;
                    break; // Bland's rule avoids degenerate cycles.
                }
            }

            if (enter == -1) return;

            i128 den = D();
            i128 sgn = (den > 0 ? 1 : -1);

            auto xnum = xNumerators();
            auto dnum = directionNumerators(enter);

            int leave = -1;
            i128 bestX = 0;
            i128 bestD = 1;

            for (int p = 0; p < 3; ++p) {
                i128 X = xnum[p] * sgn;
                i128 Dir = dnum[p] * sgn;

                if (Dir <= 0) continue;

                int cmp = (leave == -1 ? -1 : cmpFraction(X, Dir, bestX, bestD));
                if (leave == -1 ||
                    cmp < 0 ||
                    (cmp == 0 && B[p] < B[leave])) {
                    leave = p;
                    bestX = X;
                    bestD = Dir;
                }
            }

            if (leave == -1) return;

            inB[B[leave]] = 0;
            B[leave] = enter;
            inB[enter] = 1;
        }
    }
};

void solveOne() {
    int n;
    cin >> n;

    h.resize(n);

    for (int i = 0; i < n; ++i) {
        cin >> h[i].a >> h[i].b >> h[i].c;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (coverPair(i, j)) {
                cout << "YES\n2\n" << i + 1 << ' ' << j + 1 << '\n';
                return;
            }
        }
    }

    array<int, 3> basis;

    if (!findInitialBasis(basis)) {
        cout << "NO\n";
        return;
    }

    Simplex3 sp;
    sp.n = n;
    sp.B = basis;
    sp.solve();

    if (sp.normalizedObjective() < 0) {
        cout << "NO\n";
        return;
    }

    int i = sp.B[0];
    int j = sp.B[1];
    int k = sp.B[2];

    if (!coverTriple(i, j, k)) {
        cout << "NO\n";
        return;
    }

    cout << "YES\n3\n" << i + 1 << ' ' << j + 1 << ' ' << k + 1 << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    if (!(cin >> t)) return 0;
    while (t--) solveOne();

    return 0;
}
