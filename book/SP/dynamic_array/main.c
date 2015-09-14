#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "d_array.h"
#include "mem.h"

int data_del(void *data, void *ctx)
{
	P_VALID_CHECK_RET(data, -1);
	SAFE_FREE(data);
	return 0;
}

int data_print(void *data, void *ctx)
{
	printf("%d\n", *(int *)data);
	if (*(int*)ctx == 1)
		printf("\n");
	return 0;
}

int data_cmp(void *ldata, void *rdata)
{
	int res = 0;
	P_VALID_CHECK_RET(ldata, -1);
	P_VALID_CHECK_RET(rdata, -1);

	res	= *(int *)ldata - *(int*)rdata;
	if (res > 0)
		return RES_GTR;
	else if (res < 0)
		return RES_LT;
	else 
		return RES_EQUAL;
}

int main(void)
{
	DArray *a = darray_create(data_del, data_cmp, data_print, NULL);
	int i = 0, *pd = NULL;
	int data[] = {
		21,
		42,
		21,
		49,
		68,
	};

	for (i = 0; i < ARRAY_SIZE_GET(data); i++) {
		pd	= (int *)malloc(sizeof(int));
		P_VALID_CHECK_RET(pd, -1);
		darray_prepend(a, memcpy(pd, &data[i], sizeof(int)));
		pd	= (int *)malloc(sizeof(int));
		P_VALID_CHECK_RET(pd, -1);
		darray_append(a, memcpy(pd, &data[i], sizeof(int)));
	}
	darray_print(a);
	darray_sort(a, SORT_TYPE_DESC, SORT_METHOD_QK);
	darray_print(a);
	darray_unique_print(a);
	darray_destroy(a);
	return 0;
}
