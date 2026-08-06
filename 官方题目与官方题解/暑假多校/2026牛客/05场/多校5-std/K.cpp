#include <bits/stdc++.h>
using namespace std;

static inline int mex3(int x, int y, int z) {
    bool used[4] = {false, false, false, false};

    // Values outside [0, 3] cannot affect the mex of three numbers,
    // because the answer is always at most 3.
    if (0 <= x && x < 4) used[x] = true;
    if (0 <= y && y < 4) used[y] = true;
    if (0 <= z && z < 4) used[z] = true;

    for (int m = 0; m < 4; ++m) {
        if (!used[m]) return m;
    }
    return 4; // unreachable for three numbers
}

static vector<int> step_once(const vector<int>& a) {
    int n = (int)a.size();
    vector<int> b(n);

    for (int i = 0; i < n; ++i) {
        int l = (i == 0 ? n - 1 : i - 1);
        int r = (i + 1 == n ? 0 : i + 1);
        b[i] = mex3(a[l], a[i], a[r]);
    }

    return b;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long k;
    cin >> n >> k;

    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    /*
       Key fact:
       After one operation all values are in {0,1,2,3}.
       From the 3rd operation onward, the whole ring is 2-periodic:
           a^(t + 2) = a^t, for all t >= 3.
       Therefore only the first four operations are needed.
    */
    vector<vector<int>> state(5);
    state[0] = move(a);
    for (int t = 1; t <= 4; ++t) {
        state[t] = step_once(state[t - 1]);
    }

    int id;
    if (k <= 4) id = (int)k;
    else id = (k & 1LL) ? 3 : 4;

    const vector<int>& ans = state[id];
    for (int i = 0; i < n; ++i) {
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << '\n';

    return 0;
}
