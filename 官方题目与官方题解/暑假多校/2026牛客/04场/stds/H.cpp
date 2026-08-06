#include <bits/stdc++.h>
using namespace std;
 
struct Node {
    int pref;
    string suf;
    bool ok;
 
    Node(int _pref = -1, string _suf = "", bool _ok = false)
        : pref(_pref), suf(_suf), ok(_ok) {}
};
 
bool better(const Node &a, const Node &b) {
    if (!a.ok) return false;
    if (!b.ok) return true;
 
    if (a.pref != b.pref) {
        return a.pref > b.pref;
    }
 
    return a.suf < b.suf;
}
 
void upd(Node &a, const Node &b) {
    if (better(b, a)) {
        a = b;
    }
}
 
bool cmpConcat(const string &a, const string &b) {
    return a + b < b + a;
}
 
string getString(const Node &x) {
    return string(x.pref, 'a') + x.suf;
}
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int T;
    cin >> T;
 
    while (T--) {
        int n;
        cin >> n;
 
        vector<string> s(n);
        for (int i = 0; i < n; i++) {
            cin >> s[i];
        }
 
        sort(s.begin(), s.end(), cmpConcat);
 
        int L = 0;
        for (auto &x : s) {
            L += (int)x.size();
        }
 
        vector<array<Node, 2>> f(L + 1), g(L + 1);
 
        f[0][0] = Node(0, "", true);
 
        for (int i = 0; i < n; i++) {
            for (int j = 0; j <= L; j++) {
                g[j][0] = Node();
                g[j][1] = Node();
            }
 
            const string &cur = s[i];
            int m = (int)cur.size();
 
            int fullCost = 0;
            for (char c : cur) {
                fullCost += (c != 'a');
            }
 
            vector<int> preCost(m + 1, 0);
            for (int p = 0; p < m; p++) {
                preCost[p + 1] = preCost[p] + (cur[p] != 'a');
            }
 
            for (int j = 0; j <= L; j++) {
                for (int used = 0; used <= 1; used++) {
                    if (!f[j][used].ok) continue;
                    if (j + fullCost <= L) {
                        Node nxt = f[j][used];
                        nxt.pref += m;
                        upd(g[j + fullCost][used], nxt);
                    }
 
                    {
                        Node nxt = f[j][used];
                        nxt.suf += cur;
                        upd(g[j][used], nxt);
                    }
                    if (used == 0) {
                        for (int p = 0; p < m; p++) {
                            if (cur[p] == 'a') continue;
 
                            int cost = preCost[p];
                            if (j + cost > L) continue;
 
                            Node nxt;
                            nxt.ok = true;
                            nxt.pref = f[j][0].pref + p;
                            nxt.suf = cur.substr(p) + f[j][0].suf;
 
                            upd(g[j + cost][1], nxt);
                        }
                    }
                }
            }
 
            f.swap(g);
        }
 
        string best = "";
 
        for (int k = 0; k <= L; k++) {
            for (int used = 0; used <= 1; used++) {
                if (!f[k][used].ok) continue;
 
                string cur = getString(f[k][used]);
 
                if (best.empty() || cur < best) {
                    best = cur;
                }
            }
 
            cout << best << '\n';
        }
    }
 
    return 0;
}
