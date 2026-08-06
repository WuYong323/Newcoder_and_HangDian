#include <cstdio>
#include <algorithm>
using namespace std;
#define N 200000 + 5
typedef long long LL;

int n, A[N];
LL ans;

int main()
{
	scanf("%d", &n);
	for (int i = 1; i <= n; i ++)
		scanf("%d", A + i);
	for (int i = 1; i <= n; i ++)
	    ans += LL(A[i]) * (i - 1 - n + i);
	printf("%lld\n", ans);
	return 0;
}

