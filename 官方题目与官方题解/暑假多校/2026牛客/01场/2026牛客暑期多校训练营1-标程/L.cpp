#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int MOD = 998244353, MAXN = 300005;

char S[MAXN];
ll P[MAXN + 1], sumP[MAXN + 1], prefmin[MAXN + 1], sufmax[MAXN + 1];

ll rsumP(int a, int b) {
    if (a > b) return 0;
    return sumP[b] - (a ? sumP[a - 1] : 0);
}

ll norm(ll x) {
    x %= MOD;
    return x < 0 ? x + MOD : x;
}

int qnode[MAXN];

struct AC {
    int nxt[MAXN][26], fail[MAXN], L, root;
    int up[MAXN], len[MAXN];

    int newnode() {
        memset(nxt[L], -1, sizeof(nxt[L]));
        up[L] = -1;
        len[L] = 0;
        return L++;
    }

    void init() {
        L = 0;
        root = newnode();
    }

    void insert(const char *s, int id) {
        int now = root;
        for (int i = 0; s[i]; i++) {
            int c = s[i] - 'a';
            if (nxt[now][c] == -1)
                nxt[now][c] = newnode();
            now = nxt[now][c];
        }
        qnode[id] = now;
        len[now] = strlen(s);
    }

    void build() {
        queue<int> Q;
        fail[root] = root;
        for (int i = 0; i < 26; i++) {
            if (nxt[root][i] == -1)
                nxt[root][i] = root;
            else {
                fail[nxt[root][i]] = root;
                Q.push(nxt[root][i]);
            }
        }
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();
            for (int i = 0; i < 26; i++) {
                if (nxt[u][i] == -1)
                    nxt[u][i] = nxt[fail[u]][i];
                else {
                    fail[nxt[u][i]] = nxt[fail[u]][i];
                    Q.push(nxt[u][i]);
                }
            }
            up[u] = len[fail[u]] ? fail[u] : up[fail[u]];
        }
    }
} acam;

vector<int> occ[MAXN];
bool computed[MAXN];
ll ans_max[MAXN], ans_sum[MAXN];

int main() {
    int n, q;
    scanf("%d%d", &n, &q);
    scanf("%s", S + 1);
    for (int i = 1; i <= n; i++) {
        ll x;
        scanf("%lld", &x);
        P[i] = P[i - 1] + x;
    }
    sumP[0] = prefmin[0] = P[0];
    for (int i = 1; i <= n; i++) {
        sumP[i] = sumP[i - 1] + P[i];
        prefmin[i] = min(prefmin[i - 1], P[i]);
    }
    sufmax[n] = P[n];
    for (int i = n - 1; i >= 0; i--)
        sufmax[i] = max(sufmax[i + 1], P[i]);

    acam.init();
    char tbuf[MAXN];
    for (int i = 0; i < q; i++) {
        scanf("%s", tbuf);
        acam.insert(tbuf, i);
    }
    acam.build();

    int now = acam.root;
    for (int i = 1; i <= n; i++) {
        now = acam.nxt[now][S[i] - 'a'];
        for (int v = now; v > 0; v = acam.up[v]) {
            if (acam.len[v])
                occ[v].push_back(i - acam.len[v]);
        }
    }

    for (int i = 0; i < q; i++) {
        int v = qnode[i];
        if (computed[v]) {
            printf("%lld %lld\n", ans_max[v], ans_sum[v]);
            continue;
        }
        int len = acam.len[v];
        const auto &pos = occ[v];
        int m = pos.size();
        ll mx = -1e18;
        for (int k = 0; k < m; k++) {
            ll val = sufmax[pos[k] + len] - prefmin[pos[k]];
            if (val > mx) mx = val;
        }
        ll sum_val = 0;
        for (int k = 0, prev = -1; k < m; k++) {
            int p = pos[k];
            ll cntL = p - prev, cntR = n - p - len + 1;
            ll sR = norm(rsumP(p + len, n));
            ll sL = norm(rsumP(prev + 1, p));
            sum_val = (sum_val + cntL % MOD * sR - cntR % MOD * sL) % MOD;
            prev = p;
        }
        sum_val = norm(sum_val);
        ans_max[v] = mx;
        ans_sum[v] = sum_val;
        computed[v] = true;
        printf("%lld %lld\n", mx, sum_val);
    }
    return 0;
}

