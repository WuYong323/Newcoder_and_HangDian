#include<bits/stdc++.h>
#define For(i,j,k) for(int i=j;i<=k;++i)
using namespace std;
//zu he shu
int jc[15];
int c(int x,int y){ return jc[x]/(jc[x-y]*jc[y]);}
//mei jv
bool fl[2005];
int fir,mid,las,len,ma;
int q[15]; long long ans;
vector<int> seq[2005];
struct node{
    int fir,mid,las;
}var[2005];
void f(int x,int pre,int now,int nex,int w){ //ceng , shang yi ceng de s, zhe yi ceng, xia yi ceng
    if (!x) {ans+=w; return;}
    int s=q[x]-pre;
    if (s<0||s>now+nex) return;
    For(i,0,now){
        int j=s-i;
        if (j<0||j>nex) continue;
        //
        int k=c(nex,j)*c(now,i);
        f(x-1,j,mid-1,mid,w*k);
    }
}
void dfs(int x){
    //cerr<<x<<endl;
    if (x>=len+1) {
        return;
    }
    For(i,0,ma){
        q[x]=i;
        ans=0; f(x,0,las,mid,1);
        if (ans<=300&&!fl[ans]){
            fl[ans]=1;
            For(i,1,x) seq[ans].push_back(q[i]);
            var[ans]={fir,mid,las};
            // printf("%lld:",ans);
            // For(i,1,x) printf("%d ",q[i]); printf("\n");
        }
         dfs(x+1);
    }
}
void work(int x,int y,int z,int l,int m){
    fir=x,mid=y,las=z,len=l,ma=m;
    // cout<<'#'<<fir<<mid<<las<<endl;
    // cerr<<'#'<<fir<<mid<<las<<len<<endl;
    dfs(1);
}
void print(int x){
    auto [fir,mid,las]=var[x];
    int width=mid,height=seq[x].size()*2+1;
    printf("%d %d\n",height,width);
    For(i,1,width) printf("*"); printf("\n");
    for(auto i:seq[x]){
        if (width==3) printf("*");
        printf("%d",i);
        printf("*\n");
        For(i,1,width) printf("*"); printf("\n");
    }
}
int main(){
    jc[0]=1; For(i,1,10) jc[i]=jc[i-1]*i;
    work(3,2,3,8,5);
    //work(1,2,3,7,5);
    //work(1,2,1,7,5);
    work(5,3,5,5,8);
    int t; cin>>t;
    while(t--){
        int x; cin>>x; print(x);
    }
    //work(2,3,5,6,8); work(2,3,2,6,8);
    // For(i,1,300) if (!fl[i]) cerr<<i<<' ';
    // cerr<<clock()<<endl;
    return 0;
}
/*
扫雷图：n行m列，每格被翻开(有0~8的数字，表示周围雷的数量)或没被翻开(可能有雷)
构造一张图，使得雷的分布恰好有k种可能
286 194 388 316
374
47 71 89 101 103 107 142 146 197 202 214 239 251 269 311 316 317 326 358 359 362 364 374 376 386 394
*/