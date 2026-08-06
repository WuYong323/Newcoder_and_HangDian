#include<bits/stdc++.h>
using namespace std;
const long long mod=998244353;
vector<int>line[3000];
int siz[3000];
long long dp[3000][3001][2],tdp[3001][2],C[3001][3001],inv[3002];
void dfs(int t,int f){
	vector<int>::iterator it;
	int i,j;
	siz[t]=0;
	dp[t][0][0]=1;
	for(it=line[t].begin();it!=line[t].end();it++)
	{
		if(*it==f)continue;
		dfs(*it,t);
		for(i=0;i<=siz[t];i++)
		{
			for(j=0;j<=siz[*it];j++)
			{
				tdp[i+j][0]=(tdp[i+j][0]+dp[t][i][0]*dp[*it][j][0]%mod*C[i+j][i])%mod;
				tdp[i+j][1]=(tdp[i+j][1]+dp[t][i][0]*dp[*it][j][1]%mod*C[i+j][i])%mod;
				tdp[i+j][1]=(tdp[i+j][1]+dp[t][i][1]*dp[*it][j][0]%mod*C[i+j][i])%mod;
				tdp[i+j][1]=(tdp[i+j][1]+dp[t][i][0]*(dp[*it][j][1]+dp[*it][j][0]*siz[*it]%mod)%mod*i%mod*inv[j+1]%mod*C[i+j][i])%mod;
				tdp[i+j][1]=(tdp[i+j][1]+(dp[t][i][1]+dp[t][i][0]*siz[t]%mod)*dp[*it][j][0]%mod*j%mod*inv[i+1]%mod*C[i+j][i])%mod;
			}
		}
		siz[t]+=siz[*it];
		for(i=0;i<=siz[t];i++)
		{
			dp[t][i][0]=tdp[i][0];
			dp[t][i][1]=tdp[i][1];
			tdp[i][0]=0;
			tdp[i][1]=0;
		}
	}
	for(i=0;i<=siz[t];i++)dp[t][i][1]=(dp[t][i][1]+dp[t][i][0]*i)%mod;
	for(i=0;i<=siz[t];i++)
	{
		tdp[i+1][0]=(tdp[i+1][0]+dp[t][i][0])%mod;
		tdp[i+1][1]=(tdp[i+1][1]+dp[t][i][1])%mod;
	}
	siz[t]++;
	for(i=0;i<=siz[t];i++)
	{
		if(t==0)
		{
			dp[t][i][0]=tdp[i][0];
			dp[t][i][1]=tdp[i][1];
		}
		else
		{
			dp[t][i][0]=(dp[t][i][0]+tdp[i][0])%mod;
			dp[t][i][1]=(dp[t][i][1]+tdp[i][1])%mod;
		}
		tdp[i][0]=0;
		tdp[i][1]=0;
	}
}
int main(){
	ios::sync_with_stdio(false),cin.tie(0);
	int T,n,i,j,u,v;
	long long ans;
	for(i=0;i<=3000;i++)
	{
		C[i][0]=1;
		C[i][i]=1;
		for(j=1;j<i;j++)C[i][j]=(C[i-1][j-1]+C[i-1][j])%mod;
	}
	inv[1]=1;
	for(i=2;i<=3001;i++)inv[i]=(mod-mod/i)*inv[mod%i]%mod;
	for(cin>>T;T>0;T--)
	{
		cin>>n;
		for(i=0;i<n-1;i++)
		{
			cin>>u>>v;
			line[u-1].push_back(v-1);
			line[v-1].push_back(u-1);
		}
		for(i=0;i<n;i++)
		{
			for(j=0;j<=n;j++)
			{
				dp[i][j][0]=0;
				dp[i][j][1]=0;
			}
		}
		dfs(0,-1);
		ans=0;
		for(i=0;i<=n;i++)ans=(ans+dp[0][i][1])%mod;
		cout<<ans<<'\n';
		for (int i = 0; i < n; i++) {
		    line[i].clear();
		}
	}
	return 0;
}
