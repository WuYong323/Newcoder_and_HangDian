#include <bits/stdc++.h>
using namespace std;

class Solver {
public:
    explicit Solver(string s)
        : N(static_cast<int>(s.size())),
          on(N + 1, 0),
          ansY(N + 1, 0),
          ansZ(N + 1, 0) {
        for (int i = 1; i <= N; ++i) {
            on[i] = static_cast<unsigned char>(s[i - 1] - '0');
        }
    }

    void build(int n) {
        solve(n);
    }

    void print() const {
        for (int x = 1; x <= N; ++x) {
            if (x > 1) {
                cout << ' ';
            }
            cout << ansY[x];
        }
        cout << '\n';
    }

private:
    int N;
    vector<unsigned char> on;
    vector<int> ansY, ansZ;

    void flip(int x) {
        on[x] ^= 1;
    }

    // Handles positions 1..2^k-1 using only operations with indices in
    // the same range. Returns 0 if all these positions become zero;
    // otherwise returns the unique remaining one-position.
    int solve(int k) {
        if (k == 0) {
            return 0;
        }

        const int p = 1 << (k - 1);

        // Pair currently active positions in [p, 2p-1]. For a pair a,b,
        // c=a xor b lies in [1,p-1]. Operation a with (y,z)=(b,c)
        // removes a and b and only changes the lower half.
        int pending = 0;
        for (int i = p; i < 2 * p; ++i) {
            if (!on[i]) {
                continue;
            }

            if (pending == 0) {
                pending = i;
            } else {
                const int a = pending;
                const int b = i;
                const int c = a ^ b;

                ansY[a] = b;
                ansZ[a] = c;

                flip(a);
                flip(b);
                flip(c);
                pending = 0;
            }
        }

        // Operations used by this recursive call have indices < p, so
        // they are disjoint from all operation indices used above.
        const int low = solve(k - 1);

        // At this point there is at most one active position in each half.
        // If both exist, merge them into one high-half position.
        if (pending != 0 && low != 0) {
            const int z = pending ^ low;

            ansY[pending] = low;
            ansZ[pending] = z;

            flip(pending);
            flip(low);
            flip(z);
            return z;
        }

        return pending != 0 ? pending : low;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        int n;
        string s;
        cin >> n >> s;

        Solver solver(s);
        solver.build(n);
        solver.print();
    }

    return 0;
}
