#include<bits/stdc++.h>
#define For(i,j,k) for(int i=j;i<=k;++i)
using namespace std;
typedef long long ll;
const ll INF=(ll)4e18;
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
struct Seg{
    int n;
    vector<ll> mn,lz;
    void init(int m){
        n=m; mn.assign(n*4+5,INF); lz.assign(n*4+5,0);
    }
    void addtag(int p,ll v){
        if (mn[p]<INF/2) mn[p]+=v;
        lz[p]+=v;
    }
    void push(int p){
        if (!lz[p]) return ;
        addtag(p<<1,lz[p]); addtag(p<<1|1,lz[p]); lz[p]=0;
    }
    void point(int p,int l,int r,int x,ll v){
        if (l==r){mn[p]=v; lz[p]=0; return ;}
        push(p); int mid=(l+r)>>1;
        if (x<=mid) point(p<<1,l,mid,x,v);
        else point(p<<1|1,mid+1,r,x,v);
        mn[p]=min(mn[p<<1],mn[p<<1|1]);
    }
    void point(int x,ll v){point(1,1,n,x+1,v);}
    void add(int p,int l,int r,int L,int R,ll v){
        if (L<=l&&r<=R){addtag(p,v); return ;}
        push(p); int mid=(l+r)>>1;
        if (L<=mid) add(p<<1,l,mid,L,R,v);
        if (R>mid) add(p<<1|1,mid+1,r,L,R,v);
        mn[p]=min(mn[p<<1],mn[p<<1|1]);
    }
    void add(int l,int r,ll v){
        if (l>r) return ;
        add(1,1,n,l+1,r+1,v);
    }
    ll query(int p,int l,int r,int L,int R){
        if (L<=l&&r<=R) return mn[p];
        push(p); int mid=(l+r)>>1; ll res=INF;
        if (L<=mid) res=min(res,query(p<<1,l,mid,L,R));
        if (R>mid) res=min(res,query(p<<1|1,mid+1,r,L,R));
        return res;
    }
    ll query(int l,int r){
        if (l>r) return INF;
        return query(1,1,n,l+1,r+1);
    }
};
struct Solver{
    int n,m;
    vector<ll> t,x,r,dp;
    void build(vector<pair<ll,ll> > a){
        sort(a.begin(),a.end());
        vector<pair<ll,ll> > b; ll best=-(ll)4e18;
        for(int i=(int)a.size()-1;i>=0;--i){
            ll y=a[i].second-a[i].first;
            if (y>best) b.push_back(a[i]), best=y;
        }
        reverse(b.begin(),b.end());
        m=b.size(); t.assign(m+1,0); x.assign(m+1,0);
        For(i,1,m) t[i]=b[i-1].first, x[i]=b[i-1].second;
    }
    bool check(ll w){
        r.assign(m+2,0); dp.assign(m+1,INF);
        For(i,1,m){
            if (w<x[i]) return 0;
            r[i]=t[i]+w-x[i];
        }
        dp[0]=0;
        Seg seg; seg.init(m);
        set<int> good;
        struct Node{int l,r; ll h;};
        vector<Node> q;
        deque<int> mxq;
        int lb=0,le=-1;
        For(j,1,m){
            int id=j-1;
            ll base=INF;
            if (dp[id]<INF/2 && dp[id]<=r[id+1]) base=dp[id], good.insert(id);
            seg.point(id,base);
            while(!mxq.empty()&&x[mxq.back()]<=x[j]) mxq.pop_back();
            mxq.push_back(j);
            int L=id,R=id;
            while(!q.empty()&&q.back().h<=x[j]){
                seg.add(q.back().l,q.back().r,2*(x[j]-q.back().h));
                L=q.back().l; q.pop_back();
            }
            seg.add(L,R,2*x[j]);
            q.push_back({L,R,x[j]});
            while(lb<=j-1 && r[lb+1]<t[j]) ++lb;
            while(le+1<=j-1 && dp[le+1]<=t[j]) ++le;
            ll ans=INF;
            int up=min(le,j-1);
            if (lb<=up){
                auto it=good.upper_bound(up);
                if (it!=good.begin()){
                    --it;
                    if (*it>=lb){
                        int i=*it;
                        while(!mxq.empty()&&mxq.front()<=i) mxq.pop_front();
                        ans=min(ans,t[j]+2*x[mxq.front()]);
                    }
                }
            }
            int L2=max(lb,le+1),R2=j-1;
            if (L2<=R2) ans=min(ans,seg.query(L2,R2));
            dp[j]=ans;
        }
        return dp[m]<INF/2;
    }
    ll solve(){
        ll lo=0,hi=0;
        For(i,1,m) lo=max(lo,x[i]), hi=max(hi,t[i]);
        hi+=lo;
        while(lo<hi){
            ll mid=(lo+hi)>>1;
            if (check(mid)) hi=mid;
            else lo=mid+1;
        }
        return lo;
    }
};
int main(){
    int T=read();
    while(T--){
        int n=read();
        vector<ll> tv(n),xv(n);
        For(i,0,n-1) tv[i]=readll();
        For(i,0,n-1) xv[i]=readll();
        vector<pair<ll,ll> > a(n);
        For(i,0,n-1) a[i]={tv[i],xv[i]};
        Solver s; s.build(a);
        printf("%lld\n",s.solve());
    }
    return 0;
}
