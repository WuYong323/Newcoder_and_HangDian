#include <bits/stdc++.h>
using namespace std;
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int t;
    cin >> t;
 
    while (t--) {
        string S;
        int k;
        cin >> S >> k;
 
        int n = (int)S.size();
        int m = 7 + k;
 
        if (m > n) {
            cout << 0 << '\n';
            continue;
        }
 
        string base = "Rounddog";
        string doubled = S + S.substr(0, m - 1);
 
        int cnt = 0;
 
        for (int i = 0; i < n; i++) {
            if (i + m > (int)doubled.size()) break;
 
            bool ok = true;
 
            for (int j = 0; j < 8; j++) {
                if (doubled[i + j] != base[j]) {
                    ok = false;
                    break;
                }
            }
 
            if (!ok) continue;
 
            for (int j = 8; j < m; j++) {
                if (doubled[i + j] != 'g') {
                    ok = false;
                    break;
                }
            }
 
            if (ok) {
                cnt++;
                if (cnt >= 2) break;
            }
        }
 
        if (cnt == 0) {
            cout << 0 << '\n';
        } else if (cnt == 1) {
            cout << n - m + 1 << '\n';
        } else {
            cout << n << '\n';
        }
    }
 
    return 0;
}
