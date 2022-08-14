#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void fill(int a[], int n)
{
	for (int i = 0; i < n; i++)
	{
		a[i] = i + 1;
	}
}

void print(int a[], int b, int e)
{
	for (int i = b; i <= e; i++)
	{
		printf("( %2d ) ", a[i]);
	}
}

void shuffle(int a[], int n)
{
	for (int i = n - 1; i > 0; i--)
	{
		int j = rand() % (i + 1);
		int t = a[i];
		a[i] = a[j];
		a[j] = t;
	}
}

int main(void)
{
	int n = 6;
	int m = 42;
	int a[m];

	srand(time(NULL));
	
	printf("Lucky Pick Lotto 6/42\n");

	fill(a, m);
	shuffle(a, m);
	for (int j = 0; j < 6; j++)
	{
		print(a, j * 6, j * 6 + 5);
		printf("\n");
	}

	return 0;
}