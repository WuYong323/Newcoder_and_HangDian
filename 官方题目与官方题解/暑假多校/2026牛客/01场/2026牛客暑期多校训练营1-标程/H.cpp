#include <bits/stdc++.h>
using namespace std;

typedef long double db;

const int MAX_ITER = 10000;
const int H_COUNT = 10;
const int S_COUNT = 100;

struct Hand { int c[3]; };

Hand hands[H_COUNT];
int id_map[4][4][4];
db dp[MAX_ITER + 1][S_COUNT];
int nxt[S_COUNT][3][3][3][3];
db imm_win[S_COUNT][3];

inline bool beats(int a, int b) {
    return (a == 0 && b == 1) || (a == 1 && b == 2) || (a == 2 && b == 0);
}
inline int payA(int a, int b) {
    if (beats(a, b)) return 3;
    if (a == b) return 1;
    return 0;
}

void init() {
    int idx = 0;
    memset(id_map, -1, sizeof(id_map));
    for (int r = 0; r <= 3; ++r)
        for (int s = 0; s <= 3 - r; ++s) {
            int p = 3 - r - s;
            hands[idx] = {{r, s, p}};
            id_map[r][s][p] = idx++;
        }
}

void precompute() {
    for (int i = 0; i < H_COUNT; ++i)
        for (int j = 0; j < H_COUNT; ++j) {
            int s = i * H_COUNT + j;
            const Hand& A = hands[i];
            const Hand& B = hands[j];

            for (int act = 0; act < 3; ++act) {
                db w = 0.0L;
                if (A.c[act] > 0)
                    for (int b = 0; b < 3; ++b)
                        if (B.c[b] > 0 && beats(act, b))
                            w += (db)B.c[b] / 3.0L;
                imm_win[s][act] = w;
            }

            for (int act = 0; act < 3; ++act)
                for (int b = 0; b < 3; ++b)
                    for (int na = 0; na < 3; ++na)
                        for (int nb = 0; nb < 3; ++nb) {
                            int ns = -1;
                            if (A.c[act] > 0 && B.c[b] > 0) {
                                int nrA = A.c[0] - (act==0) + (na==0);
                                int nsA = A.c[1] - (act==1) + (na==1);
                                int npA = A.c[2] - (act==2) + (na==2);
                                int nrB = B.c[0] - (b==0) + (nb==0);
                                int nsB = B.c[1] - (b==1) + (nb==1);
                                int npB = B.c[2] - (b==2) + (nb==2);
                                if (nrA>=0 && nsA>=0 && npA>=0 && nrB>=0 && nsB>=0 && npB>=0) {
                                    int ni = id_map[nrA][nsA][npA];
                                    int nj = id_map[nrB][nsB][npB];
                                    if (ni != -1 && nj != -1) ns = ni * H_COUNT + nj;
                                }
                            }
                            nxt[s][act][b][na][nb] = ns;
                        }
        }
}

void solve_dp() {
    for (int s = 0; s < S_COUNT; ++s) dp[0][s] = 0.0L;
    for (int t = 1; t <= MAX_ITER; ++t)
        for (int s = 0; s < S_COUNT; ++s) {
            const Hand& A = hands[s / H_COUNT];
            const Hand& B = hands[s % H_COUNT];
            db best = -1e18L;
            bool ok = false;
            for (int act = 0; act < 3; ++act) {
                if (A.c[act] == 0) continue;
                ok = true;
                db worst = 1e18L;
                for (int b = 0; b < 3; ++b) {
                    if (B.c[b] == 0) continue;
                    db fut = 0.0L;
                    for (int na = 0; na < 3; ++na)
                        for (int nb = 0; nb < 3; ++nb) {
                            int ns = nxt[s][act][b][na][nb];
                            if (ns != -1) fut += (1.0L / 9.0L) * dp[t-1][ns];
                        }
                    db val = (db)payA(act, b) + fut;
                    if (val < worst) worst = val;
                }
                if (worst > best) best = worst;
            }
            dp[t][s] = ok ? best : 0.0L;
        }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    init();
    precompute();
    solve_dp();

    db g = 0;
    for (int s = 0; s < S_COUNT; ++s) {
        g += dp[MAX_ITER][s] - dp[MAX_ITER - 1][s];
    }
    g /= S_COUNT;

    int T;
    cin >> T;
    while (T--) {
        int k;
        string sa, sb;
        cin >> k >> sa >> sb;
        int ca[3]={0}, cb[3]={0};
        for (char c : sa) { if(c=='R')ca[0]++; else if(c=='S')ca[1]++; else ca[2]++; }
        for (char c : sb) { if(c=='R')cb[0]++; else if(c=='S')cb[1]++; else cb[2]++; }

        int idA = id_map[ca[0]][ca[1]][ca[2]];
        int idB = id_map[cb[0]][cb[1]][cb[2]];
        assert(idA >= 0 && idB >= 0);

        int s = idA * H_COUNT + idB;
        db ans = k <= MAX_ITER ? dp[k][s] : dp[MAX_ITER][s] + (k - MAX_ITER) * g;
        cout << fixed << setprecision(12) << ans << "\n";
    }
    return 0;
}

