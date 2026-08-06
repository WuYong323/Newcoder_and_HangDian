#include<bits/stdc++.h>
using namespace std;
const int INF=0x3f3f3f3f;
const int dir[4][2]={{0,1},{1,0},{0,-1},{-1,0}};
int main()
{
    int n,m;
    scanf("%d%d",&n,&m);
    vector<vector<int>> mat(n,vector<int>(m));
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            scanf("%d",&mat[i][j]);
    vector<vector<int>> dis(n,vector<int>(m,INF));
    queue<pair<int,int>> que;
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            if(mat[i][j])que.emplace(i,j),dis[i][j]=0;
    while(!que.empty())
    {
        auto [x,y]=que.front();
        que.pop();
        for(int d=0;d<4;d++)
        {
            int tx=x+dir[d][0],ty=y+dir[d][1];
            if(tx>=0 && tx<n && ty>=0 && ty<m && dis[tx][ty]>dis[x][y]+1)
                dis[tx][ty]=dis[x][y]+1,que.emplace(tx,ty);
        }
    }
    int tl=0,tr=n+m-2;
    while(tl<tr)
    {
        int tm=(tl+tr)/2;
        int lef[2]={-INF,-INF},rig[2]={INF,INF};
        for(int i=0;i<n;i++)
            for(int j=0;j<m;j++)
                if(dis[i][j]>tm)
                {
                    lef[0]=max(lef[0],i-j-tm);
                    lef[1]=max(lef[1],i+j-tm);
                    rig[0]=min(rig[0],i-j+tm);
                    rig[1]=min(rig[1],i+j+tm);
                }
        bool isok=0;
        for(int i=0;i<n;i++)
            for(int j=0;j<m;j++)
                isok|=(i-j>=lef[0] && i-j<=rig[0] && i+j>=lef[1] && i+j<=rig[1]);
        if(isok)tr=tm;
        else tl=tm+1;
    }
    return 0*printf("%d\n",tl);
}
