#include <bits/stdc++.h>
using namespace std;

class FastOutput {
    static constexpr size_t BUFFER_SIZE = 1u << 20;
    char buffer[BUFFER_SIZE];
    size_t pointer = 0;

public:
    ~FastOutput() {
        flush();
    }

    void flush() {
        if (pointer != 0) {
            fwrite(buffer, 1, pointer, stdout);
            pointer = 0;
        }
    }

    void putChar(char c) {
        if (pointer == BUFFER_SIZE) flush();
        buffer[pointer++] = c;
    }

    void writeInt(int x) {
        if (x == 0) {
            putChar('0');
            return;
        }

        char digits[16];
        int length = 0;
        while (x > 0) {
            digits[length++] = char('0' + x % 10);
            x /= 10;
        }
        while (length > 0) putChar(digits[--length]);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int B;
    if (!(cin >> B)) return 0;

    FastOutput out;

    // There is a solution exactly for even B >= 4.
    if (B < 4 || (B & 1)) {
        out.putChar('-');
        out.putChar('1');
        out.putChar('\n');
        return 0;
    }

    // The uniform construction below starts at B = 6.
    if (B == 4) {
        const int P[4] = {1, 3, 0, 2};
        const int Q[4] = {0, 1, 2, 3};
        const int R[4] = {2, 0, 3, 1};
        const int* rows[3] = {P, Q, R};

        for (const int* row : rows) {
            for (int i = 0; i < B; ++i) {
                if (i != 0) out.putChar(' ');
                out.writeInt(row[i]);
            }
            out.putChar('\n');
        }
        return 0;
    }

    const int m = B / 2;

    // A column is indexed by its P digit a.
    auto carryIn = [B, m](int a) -> int {
        if (a < m) return a != m - 2;
        return a == B - 2;
    };

    auto secondDigit = [B](int a) -> int {
        int q = a + 2;
        if (q >= B) q -= B;
        return q;
    };

    auto carryOut = [&](int a) -> int {
        const int q = secondDigit(a);
        return a + q + carryIn(a) >= B;
    };

    // Each column is a directed edge carryIn -> carryOut.
    vector<int> adjacency[2];
    adjacency[0].reserve(m);
    adjacency[1].reserve(m);

    for (int a = 0; a < B; ++a) {
        adjacency[carryIn(a)].push_back(a);
    }

    // Iterative Hierholzer. reversedCircuit is already in
    // most-significant-to-least-significant output order.
    int nextEdge[2] = {0, 0};
    vector<int> vertexStack;
    vector<int> edgeStack;
    vector<int> reversedCircuit;

    vertexStack.reserve(B + 1);
    edgeStack.reserve(B);
    reversedCircuit.reserve(B);

    vertexStack.push_back(0);
    while (!vertexStack.empty()) {
        const int state = vertexStack.back();

        if (nextEdge[state] < static_cast<int>(adjacency[state].size())) {
            const int edge = adjacency[state][nextEdge[state]++];
            edgeStack.push_back(edge);
            vertexStack.push_back(carryOut(edge));
        } else {
            vertexStack.pop_back();
            if (!edgeStack.empty()) {
                reversedCircuit.push_back(edgeStack.back());
                edgeStack.pop_back();
            }
        }
    }

    if (static_cast<int>(reversedCircuit.size()) != B) return 0;

    auto printRow = [&](int row) {
        for (int i = 0; i < B; ++i) {
            if (i != 0) out.putChar(' ');

            const int a = reversedCircuit[i];
            const int q = secondDigit(a);
            int value;

            if (row == 0) value = a;
            else if (row == 1) value = q;
            else value = (a + q + carryIn(a)) % B;

            out.writeInt(value);
        }
        out.putChar('\n');
    };

    printRow(0);
    printRow(1);
    printRow(2);
    return 0;
}
