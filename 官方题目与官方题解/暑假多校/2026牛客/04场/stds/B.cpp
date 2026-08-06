#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
void solv() {
    int p ; cin >> p;
    int x = sqrt(p) + 1;
    for(;;x++){
        ll q = 1LL * x * x - p;
        if(q < 0) continue ;
        if(q % p == 0) continue ;
        if(x % p == 0 || x % q == 0) continue ;
        
        cout << x%q <<' ' << x%p << ' ' << q << '\n' ;
        return ;
    }
}
int main() {
    ios::sync_with_stdio(false) ; cin.tie(0) ;
    int t ; cin >> t;
    while(t--) solv() ;
}
