#include <stdio.h>
#include <stdlib.h>
#include <limit.h>
#include "mylib.h"

int minMoves(int* nums, int numsSize) {
	int *dup = malloc(sizeof *dup * numsSize);
	int res = 0, max = 0, i = 0, min = 0, diff = 0;
	if (!nums || !dup)
		return 0;
	memcpy(dup, nums, numsSize * sizeof *nums);
	
	while (1) {
		for (i = 1, min = 0, max = 0; i < numsSize; i++) {
			if (dup[i] < dup[min])
				min = i;
			if (dup[i] > dup[max])
				max = i;
		}
		diff	= dup[max]-dup[min];
		if (diff == 0)
			goto ret;
		for (i = 0; i < numsSize; i++) {
			if (i != max)
				dup[i]  += diff;
		}
		res += diff;
	}
ret:
	free(dup);
	DEBUG("%d\n", res);
	return res;
}

int main(void)
{
	int nums[] = {1, INT_MAX};
	minMoves(nums, ARRAY_SIZE_GET(nums));
	return 0;
}
