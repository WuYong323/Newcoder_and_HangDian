#include <bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int t;
	cin >> t;

	while (t --) {
		int n, m;
		cin >> n >> m;
		int x = min(m + 1, n);
		cout << 1ll * x * (x - 1) / 2 - m << '\n';
	}

	return 0;
}