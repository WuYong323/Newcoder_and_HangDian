//这回只花了114514min就打完了。
//真好。记得多手造几组。ACM拍什么拍。 
#include "bits/stdc++.h"
using namespace std;
using ui = unsigned; using db = long double; using ll = long long; using ull = unsigned long long;
template<class T1, class T2> istream &operator>>(istream &cin, pair<T1, T2> &a) { return cin >> a.first >> a.second; }
template <std::size_t Index = 0, typename... Ts> typename std::enable_if<Index == sizeof...(Ts), void>::type tuple_read(std::istream &is, std::tuple<Ts...> &t) { }
template <std::size_t Index = 0, typename... Ts> typename std::enable_if < Index < sizeof...(Ts), void>::type tuple_read(std::istream &is, std::tuple<Ts...> &t) { is >> std::get<Index>(t); tuple_read<Index + 1>(is, t); }
template <typename... Ts>std::istream &operator>>(std::istream &is, std::tuple<Ts...> &t) { tuple_read(is, t); return is; }
template<class T1> istream &operator>>(istream &cin, valarray<T1> &a);
template<class T1> istream &operator>>(istream &cin, vector<T1> &a) { for (auto &x : a) cin >> x; return cin; }
template<class T1> istream &operator>>(istream &cin, valarray<T1> &a) { for (auto &x : a) cin >> x; return cin; }
template<class T1, class T2> bool cmin(T1 &x, const T2 &y) { if (y < x) { x = y; return 1; } return 0; }
template<class T1, class T2> bool cmax(T1 &x, const T2 &y) { if (x < y) { x = y; return 1; } return 0; }
template<class T1> vector<T1> range(T1 l, T1 r, T1 step = 1) { assert(step > 0); int n = (r - l + step - 1) / step, i; vector<T1> res(n); for (i = 0; i < n; i++) res[i] = l + step * i; return res; }
template<class T1> basic_string<T1> operator*(const basic_string<T1> &s, int m) { auto r = s; m *= s.size(); r.resize(m); for (int i = s.size(); i < m; i++) r[i] = r[i - s.size()]; return r; }
using lll = __int128;
using ulll = __uint128_t;
istream &operator>>(istream &cin, lll &x) { bool flg = 0; x = 0; static string s; cin >> s; if (s[0] == '-') flg = 1, s = s.substr(1); for (char c : s) x = x * 10 + (c - '0'); if (flg) x = -x; return cin; }
ostream &operator<<(ostream &cout, lll x) { static char s[60]; if (x < 0) cout << '-', x = -x; int tp = 1; s[0] = '0' + (x % 10); while (x /= 10) s[tp++] = '0' + (x % 10); while (tp--) cout << s[tp]; return cout; }
istream &operator>>(istream &cin, ulll &x) { bool flg = 0; x = 0; static string s; cin >> s; for (char c : s) x = x * 10 + (c - '0'); if (flg) x = -x; return cin; }
ostream &operator<<(ostream &cout, ulll x) { static char s[60]; int tp = 1; s[0] = '0' + (x % 10); while (x /= 10) s[tp++] = '0' + (x % 10); while (tp--) cout << s[tp]; return cout; }
#if !defined(ONLINE_JUDGE)&&defined(LOCAL)
#include "my_header/debug.h"
#else
#define dbg(...) 0
#endif
template<class T1, class T2> ostream &operator<<(ostream &cout, const pair<T1, T2> &a) { return cout << a.first << ' ' << a.second; }
template<class T1, class T2> ostream &operator<<(ostream &cout, const vector<pair<T1, T2>> &a) { for (auto &x : a) cout << x << '\n'; return cout; }
template<class T1> ostream &operator<<(ostream &cout, vector<T1> a) { int n = a.size(); if (!n) return cout; cout << a[0]; for (int i = 1; i < n; i++) cout << ' ' << a[i]; return cout; }
template<class T1> ostream &operator<<(ostream &cout, const valarray<T1> &a) { int n = a.size(); if (!n) return cout; cout << a[0]; for (int i = 1; i < n; i++) cout << ' ' << a[i]; return cout; }
template<class T1> ostream &operator<<(ostream &cout, const vector<valarray<T1>> &a) { int n = a.size(); if (!n) return cout; cout << a[0]; for (int i = 1; i < n; i++) cout << '\n' << a[i]; return cout; }
template<class T1> ostream &operator<<(ostream &cout, const vector<vector<T1>> &a) { int n = a.size(); if (!n) return cout; cout << a[0]; for (int i = 1; i < n; i++) cout << '\n' << a[i]; return cout; }
#define all(x) (x).begin(),(x).end()
#define print(...) cout<<format(__VA_ARGS__)
#define println(...) cout<<format(__VA_ARGS__)<<'\n'
#define err(...) cerr<<format(__VA_ARGS__)
#define errln(...) cerr<<format(__VA_ARGS__)<<'\n'
#define func(name, ret, ...) function<ret(__VA_ARGS__)> name = [&]( __VA_ARGS__ )
int main()
{
    ios::sync_with_stdio(0); cin.tie(0);
    cout << fixed << setprecision(15);
    int T; cin >> T;
    while (T--)
    {
        int n, i;
        cin >> n;
        vector<int> a(n), s(n + 1), id(n);
        int tot = n * (n + 1) / 2, flg = 0;
        vector<char> ban(tot + 1);
        for (i = 1; i * i <= tot; i++) ban[i * i] = 1;
        iota(all(id), 1);
        ban[tot] = 0;
        mt19937 rnd(time(0));
        bitset<5000> ed;
        do
        {
            int node = 0;
            flg = 0;
            ed.set();
            shuffle(all(id), rnd);
            function<void(int)> dfs = [&](int u) {
                if (u == n) { flg = 2; return; }
                if (++node >= n * 2) { flg = 1; return; }
                for (int i, j = ed._Find_first(); j < n; j = ed._Find_next(j)) if (u == n - 1 || !ban[tot - s[u] - id[j]])
                {
                    s[u + 1] = s[u] + id[j];
                    for (i = u - 1; i >= 0; i--) if (ban[s[u + 1] - s[i]]) break;
                    if (i >= 0) continue;
                    ed.reset(j);
                    a[u] = id[j];
                    dfs(u + 1);
                    if (flg) return;
                    ed.set(j);
                }
            };
            dfs(0);
        } while (flg != 2);
        cout << a << endl;
    }
}
