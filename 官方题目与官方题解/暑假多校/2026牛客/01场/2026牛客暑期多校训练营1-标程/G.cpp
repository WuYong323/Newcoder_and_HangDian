#include<bits/stdc++.h>
using namespace std;
typedef double db;
const db len=1e-2+1e-4;
int main()
{
    vector<pair<db,db>> loc;
    for(int i=-10;i<=10;i++)
        for(int j=-10;j<=10;j++)
        {
            db x=i+j*0.5,y=j*sqrt(3)*0.5;
            if(x*x+y*y<30)loc.push_back({x,y});
        }
    fprintf(stderr, "%zu", loc.size());
    int T;
    scanf("%d",&T);
    while(T--)
    {
        int n;
        scanf("%d",&n);
        printf("%d\n",2*n);
        for(int i=0;i<n;i++)
        {
            db x=len*loc[i].first,y=len*loc[i].second;
            printf("%.12f %.12f 0\n",x,y);
            printf("%.12f %.12f 1\n",x,y);
        }
    }
    return 0;
}

