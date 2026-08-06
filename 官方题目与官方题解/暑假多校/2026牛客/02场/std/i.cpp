#include <bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n;
	cin >> n;

	vector<array<int, 3>> vecs(n);
	for (auto &[x, y, z]: vecs) cin >> x >> y >> z;

	int q;
	cin >> q;

	vector<array<int, 5>> queries(q);
	for (auto &[u, v, w, l, r]: queries) cin >> u >> v >> w >> l >> r;

	vector<int> to_answer(q);
	iota(to_answer.begin(), to_answer.end(), 0);

	vector<long double> ans(q);

	while (!to_answer.empty()) {
		auto [dx, dy, dz, _l, _r] = queries[to_answer[0]];

		vector<int> tmp, new_to_answer;

		for (auto &idx: to_answer) {
			auto [u, v, w, l, r] = queries[idx];

			long long dot_val = 1ll * u * dx + 1ll * v * dy + 1ll * w * dz;
			long double cos2_ratio = (long double)dot_val * dot_val / (1ll * u * u + 1ll * v * v + 1ll * w * w) / (1ll * dx * dx + 1ll * dy * dy + 1ll * dz * dz);

			if (cos2_ratio > 0.991) tmp.emplace_back(idx);
			else new_to_answer.emplace_back(idx);
		}

		if (tmp.size()) {
			vector<long long> vals(n + 1);

			for (int i = 0; i < n; i ++) {
				auto [x, y, z] = vecs[i];
				long double res = 0;
				res += abs(1ll * x * dx + 1ll * y * dy + 1ll * z * dz);
				long long vx = 1ll * x * dy - 1ll * y * dx;
				long long vy = 1ll * y * dz - 1ll * z * dy;
				long long vz = 1ll * z * dx - 1ll * x * dz;
				res += hypot(vx, vy, vz);
				res /= hypot(dx, dy, dz);
				vals[i + 1] = res * (1 << 20);
			}

			for (int i = 1; i <= n; i ++) vals[i] += vals[i - 1];

			for (auto &idx: tmp) {
				auto [u, v, w, l, r] = queries[idx];
				ans[idx] = (vals[r] - vals[l - 1]) * hypot(u, v, w) / (1 << 20);
			}

			to_answer.swap(new_to_answer);
		}
	}

	cout << fixed << setprecision(10);
	for (auto &v: ans) cout << v << '\n';

	return 0;
}