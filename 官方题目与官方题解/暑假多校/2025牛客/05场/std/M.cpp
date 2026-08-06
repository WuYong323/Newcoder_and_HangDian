\
#pragma GCC optimize("O3")
#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 7;
const int inf = 0x3f3f3f3f;
int n, m, x, q;
vector<array<int, 3> > add[N], Q[N];
vector<pair<int, int> > upd[N], ask[N];
multiset<pair<int, int> > se[N];
vector<int> v;
vector<pair<int, int> > seg[N << 2];
vector<array<int, 4> > change[N << 2];

namespace IO {
#define BUF_SIZE (1 << 15)
#define OUT_SIZE (1 << 15)
#define ll long long
    // fread->read
    bool IOerror = 0;

    inline char nc() {
        static char buf[BUF_SIZE], *p1 = buf + BUF_SIZE, *pend = buf + BUF_SIZE;
        if (p1 == pend) {
            p1 = buf;
            pend = buf + fread(buf, 1, BUF_SIZE, stdin);
            if (pend == p1) {
                IOerror = 1;
                return -1;
            }
        }
        return *p1++;
    }

    inline bool blank(char ch) { return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t'; }

    inline void read(int &x) {
        bool sign = 0;
        char ch = nc();
        x = 0;
        for (; blank(ch); ch = nc());
        if (IOerror)
            return;
        if (ch == '-')
            sign = 1, ch = nc();
        for (; ch >= '0' && ch <= '9'; ch = nc())
            x = x * 10 + ch - '0';
        if (sign)
            x = -x;
    }

    inline void read(ll &x) {
        bool sign = 0;
        char ch = nc();
        x = 0;
        for (; blank(ch); ch = nc());
        if (IOerror)
            return;
        if (ch == '-')
            sign = 1, ch = nc();
        for (; ch >= '0' && ch <= '9'; ch = nc())
            x = x * 10 + ch - '0';
        if (sign)
            x = -x;
    }

    // fwrite->write
    struct Ostream_fwrite {
        char *buf, *p1, *pend;

        Ostream_fwrite() {
            buf = new char[BUF_SIZE];
            p1 = buf;
            pend = buf + BUF_SIZE;
        }

        void out(char ch) {
            if (p1 == pend) {
                fwrite(buf, 1, BUF_SIZE, stdout);
                p1 = buf;
            }
            *p1++ = ch;
        }

        void print(ll x) {
            static char s[25], *s1;
            s1 = s;
            if (!x)
                *s1++ = '0';
            if (x < 0)
                out('-'), x = -x;
            while (x)
                *s1++ = x % 10 + '0', x /= 10;
            while (s1-- != s)
                out(*s1);
        }

        void print(const char *s) {
            while (*s)
                out(*s++);
        }

        void flush() {
            if (p1 != buf) {
                fwrite(buf, 1, p1 - buf, stdout);
                p1 = buf;
            }
        }

        ~Ostream_fwrite() { flush(); }
    } Ostream;

    inline void print(ll x) { Ostream.print(x); }
    inline void print(const char *s) { Ostream.print(s); }
    inline void flush() { Ostream.flush(); }
#undef ll
#undef OUT_SIZE
#undef BUF_SIZE
}

struct BIT {
    struct node {
        int l, r;
        int mn, mx;
    } Tree[N << 2];

    void build(int k, int l, int r) {
        Tree[k].l = l;
        Tree[k].r = r;
        Tree[k].mn = inf;
        Tree[k].mx = 0;
        if (l == r)
            return;
        int mid = (l + r) >> 1;
        build(k << 1, l, mid);
        build(k << 1 | 1, mid + 1, r);
    }

    void update(int k, int pos) {
        if (Tree[k].l == Tree[k].r) {
            Tree[k].mx = Tree[k].mn = (se[pos].size() ? (*se[pos].rbegin()).first : 0);
            return;
        }
        int mid = (Tree[k].l + Tree[k].r) >> 1;
        if (pos <= mid)
            update(k << 1, pos);
        else
            update(k << 1 | 1, pos);
        Tree[k].mn = min(Tree[k << 1].mn, Tree[k << 1 | 1].mn);
        Tree[k].mx = max(Tree[k << 1].mx, Tree[k << 1 | 1].mx);
    }

    void update(int k, int pos, int v) {
        if (Tree[k].l == Tree[k].r) {
            Tree[k].mn = min(Tree[k].mn, v);
            Tree[k].mx = max(Tree[k].mx, v);
            return;
        }
        int mid = (Tree[k].l + Tree[k].r) >> 1;
        if (pos <= mid)
            update(k << 1, pos, v);
        else
            update(k << 1 | 1, pos, v);
        Tree[k].mn = min(Tree[k << 1].mn, Tree[k << 1 | 1].mn);
        Tree[k].mx = max(Tree[k << 1].mx, Tree[k << 1 | 1].mx);
    }

    int query(int k, int l, int r) {
        if (Tree[k].l >= l && Tree[k].r <= r) {
            return Tree[k].mn;
        }
        int mid = (Tree[k].l + Tree[k].r) >> 1;
        if (r <= mid)
            return query(k << 1, l, r);
        if (l > mid)
            return query(k << 1 | 1, l, r);
        return min(query(k << 1, l, r), query(k << 1 | 1, l, r));
    }

    int find(int k, int l, int r, int v) {
        if (Tree[k].mx < v)
            return x + 1;
        if (Tree[k].l == Tree[k].r)
            return Tree[k].l;
        int mid = (Tree[k].l + Tree[k].r) >> 1;
        if (l <= mid && Tree[k << 1].mx >= v)
            return find(k << 1, l, min(r, mid), v);

        if (r > mid)
            return find(k << 1 | 1, max(l, mid + 1), r, v);

        return x + 1;
    }
} T1, T4;

void clear() {
    for (int i = 1; i <= x; i++) {
        add[i].clear();
        Q[i].clear();
        upd[i].clear();
        ask[i].clear();
    }
    for (int i = 0; i < v.size(); i++) {
        seg[i].clear();
        change[i].clear();
    }
    v.clear();
}

int get(int x) {
    return lower_bound(v.begin(), v.end(), x) - v.begin();
}

int sum[N];

int low_bit(int x) {
    return x & (-x);
}

void update(int pos, int v) {
    for (int i = pos; i <= x; i += low_bit(i))
        sum[i] = min(sum[i], v);
}

int query(int pos) {
    int ans = inf;
    for (int i = pos; i; i -= low_bit(i))
        ans = min(ans, sum[i]);
    return ans;
}

void solve() {
    clock_t start = clock();
    IO::read(n);
    IO::read(m);
    IO::read(x);
    IO::read(q);
    for (int i = 1; i <= n; i++) {
        int t, l, r;
        IO::read(t);
        IO::read(l);
        IO::read(r);
        add[r - l + 1].push_back({l, r, t});
        upd[r].push_back({l, t});
    }
    vector<int> T(m + 1, inf);
    vector<pair<int, int> > a(m + 1);
    for (int i = 1; i <= m; i++) {
        int l, r, k;
        IO::read(l);
        IO::read(r);
        IO::read(k);
        Q[k].push_back({l, r, i});
        ask[r].push_back({k, i});
        a[i] = {l, r};
    }
    T1.build(1, 1, x);
    for (int k = x; k >= 1; k--) {
        for (auto [l, r, t]: add[k]) {
            T1.update(1, r, t);
        }
        for (auto [l, r, id]: Q[k]) {
            T[id] = min(T[id], T1.query(1, l + k - 1, r));
        }
    }
    for (int i = 1; i <= x; i++)
        sum[i] = inf;
    for (int i = x; i >= 1; i--) {
        for (auto [pos, v]: upd[i])
            update(pos, v);
        for (auto [k, id]: ask[i])
            T[id] = min(T[id], query(i - k + 1));
    }

    for (int i = 1; i <= m; i++)
        v.push_back(T[i]);
    vector<array<int, 4> > b(q + 1);
    vector<int> ans(q + 1, inf);
    for (int i = 1; i <= q; i++) {
        IO::read(b[i][0]);
        IO::read(b[i][1]);
        IO::read(b[i][2]);
        IO::read(b[i][3]);
        v.push_back(b[i][0]);
        v.push_back(b[i][1]);
    }
    v.push_back(inf);
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    T4.build(1, 1, x);
    int siz = v.size();
    for (int i = 1; i <= m; i++) {
        T[i] = get(T[i]);
        seg[T[i]].push_back({a[i].first, a[i].second});
    }
    for (int i = 1; i <= q; i++) {
        auto &[tl, tr, l, r] = b[i];
        tl = get(tl);
        tr = get(tr);
        change[tl].push_back({1, l, r, i});
        change[tr + 1].push_back({-1, l, r, i});
    }
    for (int i = 0; i < siz; i++) {
        for (auto [fl, l, r, id]: change[i]) {
            int premx = se[l].size() ? (*se[l].rbegin()).second : 0;
            if (fl == 1)
                se[l].insert({r, id});
            else {
                auto it = se[l].find({r, id});
                if (it != se[l].end())
                    se[l].erase(it);
            }
            int nowmx = se[l].size() ? (*se[l].rbegin()).second : 0;
            if (nowmx != premx)T4.update(1, l);
        }
        for (auto [l, r]: seg[i]) {
            while (1) {
                int pos = T4.find(1, 1, l, r);
                if (pos == x + 1)
                    break;
                int id = (*se[pos].rbegin()).second;
                ans[id] = v[i];
                se[pos].erase(*se[pos].rbegin());
                T4.update(1, pos);
            }
        }
    }
    for (int i = 1; i <= q; i++) {
        IO::print(ans[i] == inf ? -1 : ans[i]);
        IO::print("\n");
    }
    clear();
}

signed main() {
    // ios::sync_with_stdio(0);
    // cin.tie(0);
    // cout.tie(0);
    int T;
    IO::read(T);
    while (T--)
        solve();
    return 0;
}
