#include<bits/stdc++.h>
#define For(i,j,k) for(int i=j;i<=k;++i)
using namespace std;
typedef long long ll;
const ll inf=(ll)4e18;
int read(){
    int x=0,fh=1; char ch=getchar();
    for (;!isdigit(ch);ch=getchar()) if (ch=='-') fh=-1;
    for (;isdigit(ch);ch=getchar()) x=x*10+(ch^48);
    return x*fh;
}
ll readll(){
    ll x=0,fh=1; char ch=getchar();
    for (;!isdigit(ch);ch=getchar()) if (ch=='-') fh=-1;
    for (;isdigit(ch);ch=getchar()) x=x*10+(ch^48);
    return x*fh;
}
ll Abs(ll x){return x<0?-x:x;}
struct Opt{
    int k;
    ll b;
};
int n;
vector<ll> a;
ll base(int p){
    ll res=0;
    if (p>1) res+=Abs(a[p]-a[p-1]);
    if (p<n) res+=Abs(a[p]-a[p+1]);
    return res;
}
vector<Opt> getopt(int p){
    vector<ll> v;
    if (p>1) v.push_back(a[p-1]);
    if (p<n) v.push_back(a[p+1]);
    vector<Opt> res;
    int m=v.size();
    For(mask,0,(1<<m)-1){
        int k=0; ll b=0;
        For(i,0,m-1){
            int s=(mask>>i&1)?1:-1;
            k+=s; b-=1ll*s*v[i];
        }
        res.push_back((Opt){k,b});
    }
    return res;
}
ll delta_swap(int x,int y){
    vector<int> e;
    auto add=[&](int p){
        if (p>=1 && p<n) e.push_back(p);
    };
    add(x-1); add(x); add(y-1); add(y);
    sort(e.begin(),e.end());
    e.erase(unique(e.begin(),e.end()),e.end());
    ll old=0,now=0;
    for (int p:e){
        old+=Abs(a[p]-a[p+1]);
        ll u=a[p],v=a[p+1];
        if (p==x) u=a[y];
        if (p==y) u=a[x];
        if (p+1==x) v=a[y];
        if (p+1==y) v=a[x];
        now+=Abs(u-v);
    }
    return now-old;
}
void solve(){
    n=read(); a.assign(n+1,0);
    For(i,1,n) a[i]=readll();
    ll sum=0;
    For(i,1,n-1) sum+=Abs(a[i]-a[i+1]);
    ll add=0;
    ll best[5][5];
    For(i,0,4) For(j,0,4) best[i][j]=-inf;
    For(j,1,n){
        if (j>=3){
            int p=j-2;
            ll bs=base(p);
            vector<Opt> op=getopt(p);
            for (auto o:op){
                For(q,-2,2){
                    best[q+2][o.k+2]=max(best[q+2][o.k+2],o.b-bs+1ll*q*a[p]);
                }
            }
        }
        ll bs=base(j);
        vector<Opt> op=getopt(j);
        for (auto o:op){
            For(k,-2,2){
                if (best[o.k+2][k+2]==-inf) continue;
                add=max(add,best[o.k+2][k+2]+1ll*k*a[j]+o.b-bs);
            }
        }
    }
    For(i,1,n-1) add=max(add,delta_swap(i,i+1));
    printf("%lld\n",sum+add);
}
int main(){
    int T=read();
    while(T--) solve();
    return 0;
}
