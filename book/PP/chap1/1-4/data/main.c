#include <stdio.h>
#include "data.h"
#include "mem.h"

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

int main(void)
{
	FILE *fp = NULL;
	int i = 0;

	fp	= fopen(DATA_IN, "w+");
	P_VALID_CHECK_RET(fp, -1);
	
	for (i = 0; i < DATA_NUM; i++) {
		fprintf(fp, "%d\n", i);
	}

	fclose(fp);
	return 0;
}
