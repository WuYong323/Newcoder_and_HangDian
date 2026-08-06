#include<bits/stdc++.h>
#define For(i,j,k) for(int i=j;i<=k;++i)
using namespace std;
typedef long long ll;
int read(){
    int x=0,fh=1; char ch=getchar();
    for (;!isdigit(ch);ch=getchar()) if (ch=='-') fh=-1;
    for (;isdigit(ch);ch=getchar()) x=x*10+(ch^48);
    return x*fh;
}
const int B=30;
ll cnt[B][2][2];
int go(int a,int op,int b){
    if (op==1) return a&b;
    if (op==2) return a|b;
    return a^b;
}
int main(){
    int n=read();
    For(i,1,n){
        int x=read();
        For(j,0,B-1){
            int a=(x>>j)&1;
            int b=(j+1<B)?((x>>(j+1))&1):0;
            ++cnt[j][a][b];
        }
    }
    int m=read();
    while(m--){
        int op=read(),x=read();
        For(j,0,B-1){
            ll tmp[2][2]={0};
            int a=(x>>j)&1;
            int b=(j+1<B)?((x>>(j+1))&1):0;
            For(p,0,1) For(q,0,1)
                tmp[go(p,op,a)][go(q,op,b)]+=cnt[j][p][q];
            For(p,0,1) For(q,0,1) cnt[j][p][q]=tmp[p][q];
        }
        ll ans=0;
        For(j,0,B-1) ans+=cnt[j][1][0];
        printf("%lld\n",ans);
    }
    return 0;
}
