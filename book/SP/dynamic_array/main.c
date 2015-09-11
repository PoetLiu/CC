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
	printf("%s\n", (char *)data);
	return 0;
}

int data_cmp(void *ldata, void *rdata)
{
	char *l = (char *)ldata;
	char *r = (char *)rdata;
	P_VALID_CHECK_RET(l, -1);
	P_VALID_CHECK_RET(r, -1);

	if (strlen(l) > strlen(r))
		return RES_GTR;
	else if (strlen(l) < strlen(r))
		return RES_LT;
	else 
		return RES_EQUAL;
}

int main(void)
{
	DArray *a = darray_create(data_del, data_print, NULL);
	int i = 0;
	char *data[] = {
		"Hello",
		"World",
		"I",
		"AM",
		"Chinese",
	};

	for (i = 0; i < ARRAY_SIZE_GET(data); i++) {
		darray_prepend(a, strdup(data[i]));
		darray_append(a, strdup(data[i]));
	}
	darray_print(a);
	darray_sort(a, data_cmp, SORT_TYPE_DESC, SORT_METHOD_QK);
	darray_print(a);
	darray_destroy(a);
	return 0;
}
