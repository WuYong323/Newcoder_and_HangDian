// Uphill Duel -- MAIN solution.
//
// The game is a single token moving on a DAG: an edge goes from a cell to any
// orthogonally adjacent cell of strictly greater height. The player who cannot
// move loses (normal play). This is NOT a sum of independent games, so no
// Sprague-Grundy machinery is needed -- we just need the win/lose status of
// every cell:
//
//   a cell is a WINNING position (the player to move wins) iff it has at least
//   one strictly higher neighbour that is a LOSING position;
//   otherwise it is a LOSING position (in particular, a local maximum is a
//   losing position because the player to move cannot move).
//
// Because every edge goes strictly uphill, we can evaluate the cells in order
// of decreasing height: when we process a cell, all of its strictly higher
// neighbours are already evaluated. No recursion, so there is no stack-depth
// issue even for a 10^6-cell chain.
//
// Complexity: O(n*m*log(n*m)) for the ordering plus O(n*m + q) for the rest,
// summed over all test cases. There is deliberately no closed-form shortcut:
// the winner really has to be searched out over the whole height-DAG.

#include <cstdio>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

namespace fio {
    const int SZ = 1 << 16;
    char ib[SZ];
    int ip = 0, il = 0;
    inline int gc() {
        if (ip == il) {
            il = (int)fread(ib, 1, SZ, stdin);
            ip = 0;
            if (il == 0) return -1;
        }
        return ib[ip++];
    }
    inline int readInt() {
        int c = gc();
        while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
        bool neg = false;
        if (c == '-') { neg = true; c = gc(); }
        int x = 0;
        while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
        return neg ? -x : x;
    }
    char ob[SZ];
    int op = 0;
    inline void pc(char c) {
        if (op == SZ) { fwrite(ob, 1, SZ, stdout); op = 0; }
        ob[op++] = c;
    }
    inline void puts_(const char* s) { while (*s) pc(*s++); }
    inline void flush() { fwrite(ob, 1, op, stdout); op = 0; }
}

int main() {
    int T = fio::readInt();
    while (T--) {
        int n = fio::readInt();
        int m = fio::readInt();
        long long nm = (long long)n * m;

        vector<int> h((size_t)nm);
        for (long long i = 0; i < nm; i++) h[(size_t)i] = fio::readInt();

        // Order cells by decreasing height.
        vector<int> order((size_t)nm);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b) { return h[a] > h[b]; });

        vector<char> win((size_t)nm, 0);
        for (int id : order) {
            int r = id / m, c = id % m, H = h[id];
            bool w = false;
            if (r > 0)        { int nb = id - m; if (h[nb] > H && !win[nb]) w = true; }
            if (!w && r < n-1){ int nb = id + m; if (h[nb] > H && !win[nb]) w = true; }
            if (!w && c > 0)  { int nb = id - 1; if (h[nb] > H && !win[nb]) w = true; }
            if (!w && c < m-1){ int nb = id + 1; if (h[nb] > H && !win[nb]) w = true; }
            win[id] = w ? 1 : 0;
        }

        int q = fio::readInt();
        while (q--) {
            int r = fio::readInt();
            int c = fio::readInt();
            int id = (r - 1) * m + (c - 1);
            fio::puts_(win[id] ? "First\n" : "Second\n");
        }
    }
    fio::flush();
    return 0;
}
