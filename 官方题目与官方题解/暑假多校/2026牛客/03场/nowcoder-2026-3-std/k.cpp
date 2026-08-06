// Turn-by-Turn Navigation -- MAIN solution.
//
// At an intermediate waypoint P_i the arriving direction is a = P_i - P_{i-1}
// and the leaving direction is b = P_{i+1} - P_i. "Left" means b is a
// counterclockwise rotation of a (by an angle in (0, 180) degrees), which is
// exactly the sign of the 2D cross product a x b = a.x*b.y - a.y*b.x:
//   > 0  -> LEFT   (counterclockwise)
//   < 0  -> RIGHT  (clockwise)
//   = 0  -> STRAIGHT   (same direction; U-turns are excluded by the input)
//
// Coordinates are at most 10^9 in absolute value, so each direction component
// is at most 2*10^9 and the cross product is at most ~8*10^18, which fits in a
// signed 64-bit integer. No floating point is needed.
//
// Total work is O(sum of n).

#include <cstdio>
#include <vector>

using namespace std;

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int n;
        scanf("%d", &n);
        vector<long long> x(n), y(n);
        for (int i = 0; i < n; i++) scanf("%lld %lld", &x[i], &y[i]);

        for (int i = 1; i + 1 < n; i++) {
            long long ax = x[i] - x[i - 1], ay = y[i] - y[i - 1];
            long long bx = x[i + 1] - x[i], by = y[i + 1] - y[i];
            long long cross = ax * by - ay * bx;
            const char* s = (cross > 0) ? "LEFT" : (cross < 0 ? "RIGHT" : "STRAIGHT");
            if (i > 1) putchar(' ');
            fputs(s, stdout);
        }
        putchar('\n');
    }
    return 0;
}
