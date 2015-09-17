#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "data.h"
#include "mem.h"

int num[DATA_NUM];

// 获取一个在[start, end]区间内的随机整数
int randint(int start, int end)
{
	int n = 0;

	if (end == 0 || start == end)
		return -1;

	srand(time(0));
	n	= rand();
	return start + n % (end - start + 1);
}

void swap_int(int *x, int *y)
{
	P_VALID_CHECK_ACT(x, return);
	P_VALID_CHECK_ACT(y, return);

	if (*x == *y)
		return;
	*x	^= *y;
	*y	^= *x;
	*x	^= *y;
	return;
}

void num_swap(int x, int y)
{
	if (x >= DATA_NUM || x < 0 || y >= DATA_NUM || y < 0 || x == y)
		return;
	swap_int(&num[x], &num[y]);
	return;
}

int main(void)
{
	FILE *fp = NULL;
	int i = 0;

	fp	= fopen(DATA_IN, "w+");
	P_VALID_CHECK_RET(fp, -1);
	
	// 1. Generate data.
	for (i = 0; i < ARRAY_SIZE_GET(num); i++)
		num[i]	= i;

	// 2. Random data.
	for (i = 0; i < ARRAY_SIZE_GET(num); i++) {
		num_swap(i, randint(i, ARRAY_SIZE_GET(num) -1));
		fprintf(fp, "%d\n", num[i]);
	}

	fclose(fp);
	return 0;
}
