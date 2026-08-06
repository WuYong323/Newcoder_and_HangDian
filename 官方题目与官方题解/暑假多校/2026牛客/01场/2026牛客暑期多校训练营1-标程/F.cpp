#include <cstdio>
#include <algorithm>
using namespace std;
#define N 200000 + 5

int n, k, x, A[N];

int main()
{
	scanf("%d%d%d", &n, &k, &x);
	for (int i = 0; i < n; i ++)
		scanf("%d", A + i);
	int add = x + n - A[k];
	for (int i = 0; i < n; i ++)
		printf("%d%c", (A[i] + add) % n, i == n - 1 ? '\n' : ' ');
	return 0;
}

