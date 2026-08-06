#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
#define N 250000 + 5
#define M 500000 + 5

const int Fx[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
int n, m, q, sz, last, A[M], Fa[M], Size[M], Max[M];
vector<int> Id[N];

int Find(int x)
{
	if (x == Fa[x])
		return x;
	int y = Find(Fa[x]);
	Max[x] = max(Max[x], Max[Fa[x]]);
	Fa[x] = y;
	return y;
}

int main()
{
	scanf("%d%d%d", &n, &m, &q);
	for (int i = 1; i <= n; i ++)
		Id[i].resize(m + 1, 0);
	for (int i = 1, op, x, y, w; i <= q; i ++)
	{
		scanf("%d%d%d", &op, &x, &y);
#ifndef NO_CRYPT
		x ^= last, y ^= last;
#endif
		if (op == 1)
		{
			scanf("%d", &w);
			int u = ++ sz;
			A[u] = w;
			Fa[u] = Id[x][y] = u, Size[u] = 1, Max[u] = 0;
			for (int k = 0; k < 4; k ++)
			{
				int tx = x + Fx[k][0], ty = y + Fx[k][1];
				if (tx && tx <= n && ty && ty <= m && Id[tx][ty])
				{
					int v = Id[tx][ty];
					int fa_u = Find(u), fa_v = Find(v);
					if (fa_u != fa_v)
					{
						int rt = ++ sz;
						Size[rt] = Size[fa_u] + Size[fa_v];
						Max[rt] = 0;
						Fa[rt] = Fa[fa_u] = Fa[fa_v] = rt;
						Max[fa_u] = w - Size[fa_u] + 1;
						Max[fa_v] = w - Size[fa_v] + 1;
						Size[fa_u] = Size[fa_v] = 0;
					}
				}
			}
			printf("%d\n", last = Size[Find(u)] - 1);
		}
		else
		{
			Find(Id[x][y]);
			printf("%d\n", last = max(Max[Id[x][y]] - A[Id[x][y]], 0));
		}
	}
	return 0;
}

