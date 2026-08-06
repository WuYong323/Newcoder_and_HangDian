#include <bits/stdc++.h>
using namespace std;
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int T;
    cin >> T;
 
    while (T--) {
        int n;
        cin >> n;
 
        vector<int> ans;
 
        if (n == 1) {
            ans.push_back(1);
        } else if (n % 2 == 0) {
            int m = n / 2;
 
            ans.push_back(1);
            for (int i = 1; i <= m; i++) {
                ans.push_back(m + i);
                if (i < m) {
                    ans.push_back(m + 1 - i);
                }
            }
        } else {
            int m = n / 2;
 
            ans.push_back(1);
            for (int i = 1; i <= m; i++) {
                ans.push_back(m + 3 - i);
 
                if (i == 1) {
                    ans.push_back(2);
                } else {
                    ans.push_back(m + i + 1);
                }
            }
        }
 
        for (int i = 0; i < n; i++) {
            if (i) cout << ' ';
            cout << ans[i];
        }
        cout << '\n';
    }
 
    return 0;
}
