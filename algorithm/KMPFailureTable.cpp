#include <stdio.h>

void calcFailureTable(char *pat, int M, int *F) 
{
	int len = 0;
	F[0] = 0;
	
	int j = 1;
	while(j < M) 
	{
		if (pat[j] == pat[len]) 
		{
			len++;
			F[j] = len;
			j++;
		} else 
		{
			if (len != 0) 
			{
				len = F[len-1];
			} else 
			{
				F[j] = 0;
				j++;
			}
		}
	}
}

int main() { return 0; }
