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
	darray_destroy(a);
	return 0;
}
