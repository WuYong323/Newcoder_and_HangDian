#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<tuple<long long, int, int>> cells;
    cells.reserve(1LL * n * m);

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            long long value;
            cin >> value;
            cells.emplace_back(value, i, j);
        }
    }

    sort(cells.begin(), cells.end());

    int width = m + 2;
    vector<int> diff((n + 2) * width, 0);

    auto at = [&](int row, int col) -> int& {
        return diff[row * width + col];
    };

    auto addRectangle = [&](int r1, int r2, int c1, int c2) {
        ++at(r1, c1);
        --at(r2 + 1, c1);
        --at(r1, c2 + 1);
        ++at(r2 + 1, c2 + 1);
    };

    for (int start = 0; start < (int)cells.size(); ) {
        int finish = start;
        while (finish < (int)cells.size() && get<0>(cells[finish]) == get<0>(cells[start])) {
            ++finish;
        }

        vector<int> rows;
        vector<int> rowMin;
        vector<int> rowMax;

        for (int it = start; it < finish; ) {
            int row = get<1>(cells[it]);
            int mn = m + 1;
            int mx = 0;
            while (it < finish && get<1>(cells[it]) == row) {
                int col = get<2>(cells[it]);
                mn = min(mn, col);
                mx = max(mx, col);
                ++it;
            }
            rows.push_back(row);
            rowMin.push_back(mn);
            rowMax.push_back(mx);
        }

        int rowCount = (int)rows.size();
        vector<int> prefMin(rowCount), suffMax(rowCount);

        for (int i = 0; i < rowCount; ++i) {
            prefMin[i] = rowMin[i];
            if (i > 0) {
                prefMin[i] = min(prefMin[i], prefMin[i - 1]);
            }
        }

        for (int i = rowCount - 1; i >= 0; --i) {
            suffMax[i] = rowMax[i];
            if (i + 1 < rowCount) {
                suffMax[i] = max(suffMax[i], suffMax[i + 1]);
            }
        }

        for (int i = 0; i + 1 < rowCount; ++i) {
            int left = prefMin[i];
            int right = suffMax[i + 1];
            if (left < right) {
                addRectangle(rows[i], rows[i + 1], left, right);
            }
        }

        start = finish;
    }

    for (int i = 1; i <= n; ++i) {
        string rowAnswer(m, '0');
        for (int j = 1; j <= m; ++j) {
            at(i, j) += at(i - 1, j) + at(i, j - 1) - at(i - 1, j - 1);
            if (at(i, j) > 0) {
                rowAnswer[j - 1] = '1';
            }
        }
        cout << rowAnswer << '\n';
    }

    return 0;
}
