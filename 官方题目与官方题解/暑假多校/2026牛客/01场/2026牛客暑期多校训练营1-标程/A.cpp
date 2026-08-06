#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
#define N 1000000 + 5

int n;
char s[N];

bool IsVowel(char ch)
{
	return ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u'; 
}

bool IsConsonant(char ch)
{
	return !IsVowel(ch);
}

bool Check()
{
	int len = strlen(s + 1);
	if (len != 8)
		return false;
	for (int i = 1; i <= len; i ++)
	{
		if ((i & 1) && !IsConsonant(s[i]))
			return false;
		if (((i + 1) & 1) && !IsVowel(s[i]))
			return false;
	}
	return true;
}

int main()
{
	for (scanf("%d", &n); n; n --)
	{
		scanf("%s", s + 1);
		puts(Check() ? "Suspected Virus" : "Well-Being");
	}
	return 0;
}

