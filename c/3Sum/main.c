#include <stdio.h>
#include <stdlib.h>

/**
 * Return an array of arrays of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
void addThree(int **res, int *size, int i, int j, int k)
{
	int *p = NULL;
	if (!res)
		return;

	(*size) += 3; 
	if (*res == NULL)
		p   = malloc(3*sizeof(int));
	else
		p   = realloc(*res, (*size)*sizeof(int));
	if (!p) {
		*res    = NULL;
		return;
	}

set:
	p[*size-3]  = i;
	p[*size-2]  = j;
	p[*size-1]  = k;
	*res    = p;
	return;
}

int* threeSum(int* nums, int numsSize, int* returnSize) {
	int i = 0, j = 0, k = 0, size = 0;
	int *res = NULL;

	for (i = 0; i < numsSize; i++) {
		for (j = 0; j < numsSize && j != i; j++) {
			for (k = 0; k < numsSize && j != i && j != k; k++) {
				if (nums[i] + nums[j] + nums[k] == 0) {
					printf("i:%d j:%d k:%d found match!\n", nums[i], nums[j], nums[k]);
					addThree(&res, &size, nums[i], nums[j], nums[k]);
				}
			}
		}
	}
	*returnSize = size;
	return res;
}

int main(int argc, char *argv[])
{
	int test[] = {-1, 0, 1, 2, -1, 4
	}; 

	int num = 0, i = 0;
	int *p = NULL;


	p = threeSum(test, sizeof(test)/sizeof(int), &num);
	printf("found num:%d\n", num);
	for (i = 0; i < num; i++) {
		if (i%3 == 0 && i != 0)
			printf("\n");
		printf("%d ", p[i]);
	}


	return 0;
}
