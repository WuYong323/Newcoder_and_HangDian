#include <bits/stdc++.h>

using namespace std;

int c = 1;

unordered_map<string, int> mp;
unordered_map<int, string> pm;

struct Info {
    int nq, w, d;
    Info() : nq(0), w(0), d(0) { }
    Info(int nq, int w, int d) : nq(nq), w(w), d(d) { }
};

vector<array<Info, 2>> trans = {{Info(), Info()}};

int id(const string& s) {
    if (!mp.count(s)) pm[c] = s, mp[s] = c++, trans.push_back({Info(), Info()});
    return mp[s];
}

constexpr int B = 16;
constexpr int halt = 0;
int p0, q0;

void init() {
    p0 = B;
    q0 = id(format("init copy left {} {} {}", 0, B, 0));
    for (int i = 0; i < B; i++) {
        for (int l = 0; l < 2; l++) {
            int j = B;
            trans[id(format("init copy left {} {} {}", i, j, 0))][l] = 
                {id(format("init copy left {} {} {}", i, j - 1, l)), l, -1};
        }
        for (int j = B - 1; j > 0; j--) for (int k = 0; k < 2; k++) for (int l = 0; l < 2; l++) {
            trans[id(format("init copy left {} {} {}", i, j, k))][l] = 
                {id(format("init copy left {} {} {}", i, j - 1, k)), l, -1};
        }
        for (int k = 0; k < 2; k++) {
            int j = 0;
            trans[id(format("init copy left {} {} {}", i, j, k))][0] = 
                {id(format("init copy right {} {}", i, 1)), k, 1};
        }
        for (int j = 1; j < B; j++) for (int k = 0; k < 2; k++) {
            trans[id(format("init copy right {} {}", i, j))][k] = 
                {id(format("init copy right {} {}", i, j + 1)), k, 1};
        }
        for (int k = 0; k < 2; k++) {
            int j = B;
            if (i == B - 1) {
                trans[id(format("init copy right {} {}", i, j))][k] = 
                    {id(format("init move to start {}", 2 * B - 2)), k, -1};
            } else {
                trans[id(format("init copy right {} {}", i, j))][k] = 
                    {id(format("init copy left {} {} {}", i + 1, B, 0)), k, 1};
            }
        }
    }
    for (int j = 2 * B - 2; j > 0; j--) for (int k = 0; k < 2; k++) {
        trans[id(format("init move to start {}", j))][k] = 
            {id(format("init move to start {}", j - 1)), k, -1};
    }
    for (int k = 0; k < 2; k++) {
        trans[id("init move to start 0")][k] = 
            {id(format("forward check zero {} {}", 0, 0)), k, 0};
    }
}

void forward() {
    for (int i = 0; i < B; i++) {
        trans[id(format("forward check zero {} {}", i, 0))][0] = 
            {id(format("forward check zero {} {}", i + 1, 0)), 1, 1};
        trans[id(format("forward check zero {} {}", i, 0))][1] = 
            {id(format("forward check zero {} {}", i + 1, 1)), 0, 1};
        trans[id(format("forward check zero {} {}", i, 1))][0] = 
            {id(format("forward check zero {} {}", i + 1, 1)), 0, 1};
        trans[id(format("forward check zero {} {}", i, 1))][1] = 
            {id(format("forward check zero {} {}", i + 1, 1)), 1, 1};
    } 
    for (int k = 0; k < 2; k++) {
        trans[id(format("forward check zero {} {}", B, 0))][k] = 
            {id(format("forward end fill zero {}", B - 1)), k, -1};
    }
    for (int j = B - 1; j > 0; j--) for (int k = 0; k < 2; k++) {
        trans[id(format("forward end fill zero {}", j))][k] = 
            {id(format("forward end fill zero {}", j - 1)), 0, -1};
    }
    for (int k = 0; k < 2; k++) {
        int j = 0;
        trans[id(format("forward end fill zero {}", j))][k] = 
            {id(format("backward move to check zero 0")), 0, 0};
    }
    for (int k = 0; k < 2; k++) {
        trans[id(format("forward check zero {} {}", B, 1))][k] = 
            {id(format("forward before swap {}", B + 1)), k, 1};
    }
    for (int j = B + 1; j < 4 * B - 1; j++) for (int k = 0; k < 2; k++) {
        trans[id(format("forward before swap {}", j))][k] = 
            {id(format("forward before swap {}", j + 1)), k, 1};
    }
    for (int k = 0; k < 2; k++) {
        int j = 4 * B - 1;
        trans[id(format("forward before swap {}", j))][k] = 
            {id(format("forward swap left {} {} {}", 0, 4 * B - 2, k)), k, -1};
    }
    for (int i = 0; i < B; i++) {
        for (int j = 4 * B - 2; j > 0; j--) for (int k = 0; k < 2; k++) for (int l = 0; l < 2; l++) {
            trans[id(format("forward swap left {} {} {}", i, j, k))][l] = 
                {id(format("forward swap left {} {} {}", i, j - 1, k)), l, -1};
        }
        for (int k = 0; k < 2; k++) for (int l = 0; l < 2; l++) {
            int j = 0;
            trans[id(format("forward swap left {} {} {}", i, j, k))][l] = 
                {id(format("forward swap right {} {} {}", i, j + 1, l)), k, 1};
        }
        for (int j = 1; j < 4 * B - 1; j++) for (int k = 0; k < 2; k++) for (int l = 0; l < 2; l++) {
            trans[id(format("forward swap right {} {} {}", i, j, k))][l] = 
                {id(format("forward swap right {} {} {}", i, j + 1, l)), k, 1};
        }
        for (int k = 0; k < 2; k++) for (int l = 0; l < 2; l++) {
            int j = 4 * B - 1;
            if (i == B - 1) {
                trans[id(format("forward swap right {} {} {}", i, j, k))][l] = 
                    {id(format("forward move to start {}", 3 * B - 2)), k, -1};
            } else {
                trans[id(format("forward swap right {} {} {}", i, j, k))][l] = 
                    {id(format("forward swap left {} {} {}", i + 1, j - 1, k)), k, -1};
            }
        }
    }
    for (int j = 3 * B - 2; j > 0; j--) for (int k = 0; k < 2; k++) {
        trans[id(format("forward move to start {}", j))][k] = 
            {id(format("forward move to start {}", j - 1)), k, -1};
    }
    for (int k = 0; k < 2; k++) {
        int j = 0;
        trans[id(format("forward move to start {}", j))][k] = 
            {id(format("forward check zero {} {}", 0, 0)), k, 0};
    }
}

void backward() {
    for (int j = 0; j < B - 1; j++) for (int k = 0; k < 2; k++) {
        trans[id(format("backward move to check zero {}", j))][k] = 
            {id(format("backward move to check zero {}", j + 1)), k, 1};
    }
    for (int k = 0; k < 2; k++) {
        int j = B - 1;
        trans[id(format("backward move to check zero {}", j))][k] = 
            {id(format("backward check zero {} {}", 0, 0)), k, 1};
    }
    for (int i = 0; i < B; i++) {
        trans[id(format("backward check zero {} {}", i, 0))][0] = 
            {id(format("backward check zero {} {}", i + 1, 0)), 1, 1};
        trans[id(format("backward check zero {} {}", i, 0))][1] = 
            {id(format("backward check zero {} {}", i + 1, 1)), 0, 1};
        trans[id(format("backward check zero {} {}", i, 1))][0] = 
            {id(format("backward check zero {} {}", i + 1, 1)), 0, 1};
        trans[id(format("backward check zero {} {}", i, 1))][1] = 
            {id(format("backward check zero {} {}", i + 1, 1)), 1, 1};
    } 
    for (int k = 0; k < 2; k++) {
        trans[id(format("backward check zero {} {}", B, 0))][k] = 
            {halt, 0, 0};
        trans[id(format("backward check zero {} {}", B, 1))][k] = 
            {id(format("backward before swap {}", 3 * B - 1)), k, -1};
    }
    for (int j = 3 * B - 1; j > 0; j--) for (int k = 0; k < 2; k++) {
        trans[id(format("backward before swap {}", j))][k] = 
            {id(format("backward before swap {}", j - 1)), k, -1};
    }
    for (int k = 0; k < 2; k++) {
        int j = 0;
        trans[id(format("backward before swap {}", j))][k] = 
            {id(format("backward swap right {} {} {}", 0, 1, k)), k, 1};
    }
    for (int i = 0; i < B; i++) {
        for (int j = 1; j < 4 * B - 1; j++) for (int k = 0; k < 2; k++) for (int l = 0; l < 2; l++) {
            trans[id(format("backward swap right {} {} {}", i, j, k))][l] = 
                {id(format("backward swap right {} {} {}", i, j + 1, k)), l, 1};
        }
        for (int k = 0; k < 2; k++) for (int l = 0; l < 2; l++) {
            int j = 4 * B - 1;
            trans[id(format("backward swap right {} {} {}", i, j, k))][l] = 
                {id(format("backward swap left {} {} {}", i, j - 1, l)), k, -1};
        }
        for (int j = 4 * B - 2; j > 0; j--) for (int k = 0; k < 2; k++) for (int l = 0; l < 2; l++) {
            trans[id(format("backward swap left {} {} {}", i, j, k))][l] = 
                {id(format("backward swap left {} {} {}", i, j - 1, l)), k, -1};
        }
        for (int k = 0; k < 2; k++) for (int l = 0; l < 2; l++) {
            int j = 0;
            if (i == B - 1) {
                trans[id(format("backward swap left {} {} {}", i, j, k))][l] = 
                    {id(format("backward move to compare {}", 1)), k, 1};
            } else {
                trans[id(format("backward swap left {} {} {}", i, j, k))][l] = 
                    {id(format("backward swap right {} {} {}", i + 1, j + 1, k)), k, 1};
            }
        }
    }
    for (int j = 1; j < 3 * B - 1; j++) for (int k = 0; k < 2; k++) {
        trans[id(format("backward move to compare {}", j))][k] = 
            {id(format("backward move to compare {}", j + 1)), k, 1};
    }
    for (int k = 0; k < 2; k++) {
        int j = 3 * B - 1;
        trans[id(format("backward move to compare {}", j))][k] = 
            {id(format("backward compare start {}", B - 1)), k, 0};
    }
    for (int j = 1; j < 2 * B; j++) for (int k = 0; k < 2; k++) for (int l = 0; l < 2; l++) {
        trans[id(format("backward compare res move back {} {}", j, k))][l] = 
            {id(format("backward compare res move back {} {}", j - 1, k)), l, -1};
    }
    for (int k = 0; k < 2; k++) for (int l = 0; l < 2; l++) {
        int j = 0;
        trans[id(format("backward compare res move back {} {}", j, k))][l] = 
            {id(format("backward compare finish move to start {} {}", 2 * B - 1, k)), l, -1};
    }
    for (int j = 2 * B - 1; j > 0; j--) for (int k = 0; k < 2; k++) for (int l = 0; l < 2; l++) {
        trans[id(format("backward compare finish move to start {} {}", j, k))][l] = 
            {id(format("backward compare finish move to start {} {}", j - 1, k)), l, -1};
    }
    for (int l = 0; l < 2; l++) {
        int j = 0;
        trans[id(format("backward compare finish move to start {} {}", j, 0))][l] = 
            {id(format("backward move to check zero {}", 0)), l, 0};
        trans[id(format("backward compare finish move to start {} {}", j, 1))][l] = 
            {id(format("backward increment {} {}", 0, 0)), l, 0};
    }
    for (int i = 0; i < B; i++) {
        trans[id(format("backward increment {} {}", i, 0))][0] = 
            {id(format("backward increment {} {}", i + 1, 1)), 1, 1};
        trans[id(format("backward increment {} {}", i, 0))][1] = 
            {id(format("backward increment {} {}", i + 1, 0)), 0, 1};
        trans[id(format("backward increment {} {}", i, 1))][0] = 
            {id(format("backward increment {} {}", i + 1, 1)), 0, 1};
        trans[id(format("backward increment {} {}", i, 1))][1] = 
            {id(format("backward increment {} {}", i + 1, 1)), 1, 1};
    } 
    for (int k = 0; k < 2; k++) {
        trans[id(format("backward increment {} {}", B, 1))][k] =
            {id(format("backward check zero {} {}", 0, 0)), k, 0}; 
    }
    for (int i = B - 1; i >= 0; i--) {
        for (int k = 0; k < 2; k++) {
            trans[id(format("backward compare start {}", i))][k] = 
                {id(format("backward compare right {} {} {}", i, 1, k)), k, 1};
        }
        for (int j = 1; j < B; j++) for (int k = 0; k < 2; k++) for (int l = 0; l < 2; l++) {
            trans[id(format("backward compare right {} {} {}", i, j, k))][l] = 
                {id(format("backward compare right {} {} {}", i, j + 1, k)), l, 1};
        }
        for (int k = 0; k < 2; k++) for (int l = 0; l < 2; l++) {
            int j = B;
            if (k < l) {
                trans[id(format("backward compare right {} {} {}", i, j, k))][l] = 
                    {id(format("backward compare res move back {} {}", B + i, 1)), l, 0};
            } else if (k > l || i == 0) {
                trans[id(format("backward compare right {} {} {}", i, j, k))][l] = 
                    {id(format("backward compare res move back {} {}", B + i, 0)), l, 0};
            } else {
                trans[id(format("backward compare right {} {} {}", i, j, k))][l] = 
                    {id(format("backward compare equal next {} {}", i, B)), l, -1};
            }
        }
        if (i != 0) {
            for (int j = B; j > 0; j--) for (int k = 0; k < 2; k++) {
                trans[id(format("backward compare equal next {} {}", i, j))][k] = 
                    {id(format("backward compare equal next {} {}", i, j - 1)), k, -1};
            }
            for (int k = 0; k < 2; k++) {
                int j = 0;
                trans[id(format("backward compare equal next {} {}", i, j))][k] = 
                    {id(format("backward compare start {}", i - 1)), k, 0};
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    init();
    forward();
    backward();

    cout << c << "\n";
    for (int i = 0; i < c; i++) {
        cout << trans[i][0].nq << " " << trans[i][0].w << " " << trans[i][0].d << " ";
        cout << trans[i][1].nq << " " << trans[i][1].w << " " << trans[i][1].d << "\n";
    }
    cout << q0 << " " <<  halt << " " << p0 << "\n";
}