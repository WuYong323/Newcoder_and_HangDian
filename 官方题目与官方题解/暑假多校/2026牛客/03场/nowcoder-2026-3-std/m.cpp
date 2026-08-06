#include<bits/stdc++.h>
#define For(i,j,k) for(int i=j;i<=k;++i)
using namespace std;
typedef long long ll;
const int mod=998244353;
int read(){
    int x=0,fh=1; char ch=getchar();
    for (;!isdigit(ch);ch=getchar()) if (ch=='-') fh=-1;
    for (;isdigit(ch);ch=getchar()) x=x*10+(ch^48);
    return x*fh;
}
ll ksm(ll x,ll p=mod-2){
    ll ans=1;
    while(p){
        if (p&1) ans=ans*x%mod;
        x=x*x%mod; p>>=1;
    }
    return ans;
}
int main(){
    int T=read();
    while(T--){
        int n=read(),s=read(),t=read();
        vector<vector<pair<int,int> > > e(n+1),son(n+1);
        For(i,2,n){
            int x=read(),id=2*(i-2);
            e[i].push_back({x,id});
            e[x].push_back({i,id^1});
        }
        if (s==t){
            puts("0");
            continue;
        }
        vector<int> fa(n+1,-1),fe(n+1,-1),ord;
        ord.reserve(n); ord.push_back(t); fa[t]=0;
        for(int i=0;i<(int)ord.size();++i){
            int x=ord[i];
            for(auto it:e[x]){
                int y=it.first,id=it.second;
                if (fa[y]!=-1) continue;
                fa[y]=x; fe[y]=id;
                son[x].push_back({y,id});
                ord.push_back(y);
            }
        }
        vector<ll> A(n+1),B(n+1);
        for(int ii=n-1;ii>=0;--ii){
            int x=ord[ii];
            if (x==t) continue;
            int k=son[x].size();
            if (!k){
                A[x]=B[x]=1;
                continue;
            }
            ll D=k,sumB=0,sumAp=0,sumAq0=0,sumAq1=0;
            vector<ll> p(k),q0(k),q1(k);
            For(i,0,k-1){
                int y=son[x][i].first;
                sumB=(sumB+B[y])%mod;
            }
            For(i,0,k-1){
                int y=son[x][i].first;
                p[i]=ksm((D+A[y])%mod);
                q1[i]=p[i];
                q0[i]=(D+sumB-B[y]+mod)%mod*p[i]%mod;
                sumAp=(sumAp+A[y]*p[i])%mod;
                sumAq0=(sumAq0+A[y]*q0[i])%mod;
                sumAq1=(sumAq1+A[y]*q1[i])%mod;
            }
            ll inv=ksm((1-sumAp+mod)%mod);
            ll S0=sumAq0*inv%mod,S1=sumAq1*inv%mod;
            ll sx0=0,sx1=0;
            For(i,0,k-1){
                int y=son[x][i].first;
                ll y0=(p[i]*S0+q0[i])%mod;
                ll y1=(p[i]*S1+q1[i])%mod;
                sx0=(sx0+A[y]*y0+B[y])%mod;
                sx1=(sx1+A[y]*y1)%mod;
            }
            ll invD=ksm(D);
            A[x]=sx1*invD%mod;
            B[x]=(1+sx0*invD)%mod;
        }
        vector<ll> val(2*(n-1)),out(n+1);
        for(auto it:son[t]){
            int y=it.first,id=it.second;
            out[y]=0;
            val[id]=B[y];
            val[id^1]=0;
        }
        for(int x:ord){
            if (x==t) continue;
            val[fe[x]]=(A[x]*out[x]+B[x])%mod;
            val[fe[x]^1]=out[x];
            int k=son[x].size();
            if (!k) continue;
            ll D=k,sumB=0,sumAp=0,sumAq=0;
            vector<ll> p(k),q(k);
            For(i,0,k-1){
                int y=son[x][i].first;
                sumB=(sumB+B[y])%mod;
            }
            For(i,0,k-1){
                int y=son[x][i].first;
                p[i]=ksm((D+A[y])%mod);
                q[i]=(D+out[x]+sumB-B[y]+mod)%mod*p[i]%mod;
                sumAp=(sumAp+A[y]*p[i])%mod;
                sumAq=(sumAq+A[y]*q[i])%mod;
            }
            ll S=sumAq*ksm((1-sumAp+mod)%mod)%mod;
            For(i,0,k-1){
                int y=son[x][i].first,id=son[x][i].second;
                ll yy=(p[i]*S+q[i])%mod;
                out[y]=yy;
                val[id]=(A[y]*yy+B[y])%mod;
                val[id^1]=yy;
            }
        }
        ll sum=0;
        for(auto it:e[s]){
            int id=it.second;
            sum=(sum+val[id])%mod;
        }
        printf("%lld\n",(1+sum*ksm((int)e[s].size()))%mod);
    }
    return 0;
}
