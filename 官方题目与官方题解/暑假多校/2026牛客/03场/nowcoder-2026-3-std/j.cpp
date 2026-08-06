#include <bits/stdc++.h>
using namespace std;

struct HeapNode {
    int value;
    int left;
    int right;
    int dist;
};

vector<HeapNode> heapNodes(1);
vector<int> originalDepth;

bool higherPriority(int a, int b) {
    if (b == 0) {
        return true;
    }
    if (a == 0) {
        return false;
    }
    int va = heapNodes[a].value;
    int vb = heapNodes[b].value;
    if (originalDepth[va] != originalDepth[vb]) {
        return originalDepth[va] > originalDepth[vb];
    }
    return va > vb;
}

int mergeHeap(int a, int b) {
    if (a == 0) {
        return b;
    }
    if (b == 0) {
        return a;
    }
    if (!higherPriority(a, b)) {
        swap(a, b);
    }

    heapNodes[a].right = mergeHeap(heapNodes[a].right, b);
    if (heapNodes[heapNodes[a].left].dist < heapNodes[heapNodes[a].right].dist) {
        swap(heapNodes[a].left, heapNodes[a].right);
    }
    heapNodes[a].dist = heapNodes[heapNodes[a].right].dist + 1;
    return a;
}

int newHeapNode(int value) {
    heapNodes.push_back({value, 0, 0, 1});
    return (int)heapNodes.size() - 1;
}

int popHeap(int root) {
    return mergeHeap(heapNodes[root].left, heapNodes[root].right);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> originalParent(n + 1, 0);
    originalDepth.assign(n + 1, 0);

    for (int i = 2; i <= n; ++i) {
        cin >> originalParent[i];
        originalDepth[i] = originalDepth[originalParent[i]] + 1;
    }

    vector<int> heapRoot(n + 1, 0);
    heapNodes.reserve(q + 1);

    for (int i = 0; i < q; ++i) {
        int u, v;
        cin >> u >> v;
        heapRoot[u] = mergeHeap(heapRoot[u], newHeapNode(v));
    }

    vector<int> order(n);
    iota(order.begin(), order.end(), 1);
    sort(order.begin(), order.end(), [&](int a, int b) {
        return originalDepth[a] > originalDepth[b];
    });

    vector<int> newParent(n + 1, 1);
    newParent[1] = 0;

    for (int x : order) {
        if (x == 1) {
            continue;
        }

        int root = heapRoot[x];
        if (root == 0) {
            newParent[x] = 1;
            continue;
        }

        int deepest = heapNodes[root].value;
        newParent[x] = deepest;

        while (root != 0 && heapNodes[root].value == deepest) {
            root = popHeap(root);
        }

        heapRoot[x] = 0;
        if (root != 0) {
            heapRoot[deepest] = mergeHeap(heapRoot[deepest], root);
        }
    }

    sort(order.begin(), order.end(), [&](int a, int b) {
        return originalDepth[a] < originalDepth[b];
    });

    vector<long long> newDepth(n + 1, 0);
    long long answer = 0;

    for (int x : order) {
        if (x == 1) {
            continue;
        }
        newDepth[x] = newDepth[newParent[x]] + 1;
        answer += newDepth[x];
    }

    cout << answer << '\n';
    return 0;
}
