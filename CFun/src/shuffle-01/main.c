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

void print(int a[], int n)
{
	for (int i = 0; i < n; i++)
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

struct cnt_t {
	int n;
	int c;
};

int cmp(void *a,void *b) {
	struct cnt_t l=*(struct cnt_t*)a;	
	struct cnt_t r=*(struct cnt_t*)b;
	if(l.c<r.c) return 1;
	else if(l.c>r.c) return -1;
	return 0;	
}

int main(void)
{
	int w[4];
	int n = 6;
	int m = 42;
	int a[m];
	int d=1000000;
	struct cnt_t c[m];

	int b[] = {4, 1, 2, 40, 18, 3};

	srand(time(NULL));

	for (int i = 0; i < 4; i++)
	{
		w[i] = 0;
	}

	for (int i = 0; i < m; i++)
	{
		c[i].n = 0;
		c[i].c = 0;
	}

	for (int i = 0; i < d; i++)
	{
		printf("#%7d ",i+1);
		
		fill(a, m);

		shuffle(a, m);
		
		print(a, n);

		for(int j=0;j<n;j++) {
			c[a[j]-1].n=a[j];
			c[a[j]-1].c++;
		}

		int s = 0;
		for (int k = 0; k < n - 1; k++)
		{
			for (int j = k + 1; j < n; j++)
			{
				if (a[j] == b[k])
				{
					s++;
				}
			}
		}

		switch (s)
		{
		case 6:
			printf("jackpot");
			w[0]++;
			break;
		case 5:
			printf("2nd prize");
			w[1]++;
			break;
		case 4:
			printf("3rd prize");
			w[2]++;
			break;
		case 3:
			printf("4th prize");
			w[3]++;
			break;
		default:
			printf("malaska");
			w[4]++;
		}

		printf("\n");
		
	}

	printf("\n");
	
	qsort(c,m,sizeof(*c),cmp);	

	printf("Top 6 numbers:\n");
	for(int i=0;i<m;i++) {
		printf("%2d -> %7d %8.4f%%\n",c[i].n,c[i].c,(float)c[i].c/d*6*100);
		if(i==5) printf("\nOther numbers:\n");
	}
	
	printf("\n");

	printf("jackpot   -> %7d %8.4f%%\n",w[0],(float)w[0]/d*100);
	printf("2nd prize -> %7d %8.4f%%\n",w[1],(float)w[1]/d*100);
	printf("3rd prize -> %7d %8.4f%%\n",w[2],(float)w[2]/d*100);
	printf("4th prize -> %7d %8.4f%%\n",w[3],(float)w[3]/d*100);
	printf("malaska   -> %7d %8.4f%%\n",w[4],(float)w[4]/d*100);

	printf("\n");
	

	return 0;
}
