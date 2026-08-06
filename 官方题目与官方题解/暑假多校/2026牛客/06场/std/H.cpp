#include <bits/stdc++.h>

using namespace std;

constexpr int N = 2e5;

int f[N];

void init() {
    fill(f, f + N, 1);
    f[1] = 0;
    for (int i = 2; i < N; i++) if (f[i]) for (int j = i + i; j < N; j += i) f[j] = 0;
}

void solve() {
    int n;
    cin >> n;
    if (n == 3 || n == 4 || n == 6) return void(cout << -1 << "\n");
    if (f[n - 1]) {
        for (int i = 1; i <= n - 4; i++) cout << i << " ";
        for (int i = n; i > n - 4; i--) cout << i << " \n"[i == n - 3];
    } else {
        for (int i = 1; i <= n; i++) cout << i << " \n"[i == n];
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    init();
    int t;
    cin >> t;
    while (t--) solve();
}