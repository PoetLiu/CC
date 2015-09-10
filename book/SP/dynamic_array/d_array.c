#include <stdlib.h>
#include "mem.h"
#include "debug.h"
#include "d_array.h"

#define MIN_PRE_ALLOCATE_NR	10
struct _DArray {
	size_t size;
	size_t alloc_size;
	DataDestroyFunc destroy;
	DataPrintFunc print;
	void **data;
};

static int darray_expand(DArray *thiz, size_t need)
{
	P_VALID_CHECK_RET(thiz, -1);

	if ((thiz->size + need) > thiz->alloc_size) {
		size_t alloc_size = thiz->alloc_size + (thiz->alloc_size >> 1) + MIN_PRE_ALLOCATE_NR;
		void **data = (void **)realloc(thiz->data, sizeof(void*) * alloc_size);
		if (data != NULL) {
			thiz->data		= data;	
			thiz->alloc_size	= alloc_size;
		}
	}
	return ((thiz->size + need) <= thiz->alloc_size) ? 0 : -1;
}

static int darray_shrink(DArray *thiz)
{
	P_VALID_CHECK_RET(thiz, -1);
	if ((thiz->size < (thiz->alloc_size >> 1)) && (thiz->alloc_size > MIN_PRE_ALLOCATE_NR)) {
		size_t	alloc_size = thiz->size + (thiz->size >> 1);	
		void **data = (void **)realloc(thiz->data, sizeof(void*) * alloc_size);
		if (data != NULL) {
			thiz->data		= data;	
			thiz->alloc_size	= alloc_size;
		}
	}
	return 0;
}

int darray_foreach(DArray *thiz, DataVisitFunc visit, void *ctx)
{
	int i = 0;
	P_VALID_CHECK_RET(thiz, -1);
	P_VALID_CHECK_RET(visit, -1);

	for (i = 0; i < thiz->size; i++)
		visit(thiz->data[i], ctx);
	return 0;
}

int darray_print(DArray *thiz)
{
	return thiz && thiz->print ? darray_foreach(thiz, thiz->print, NULL) : -1;
}

DArray* darray_create(DataDestroyFunc data_destroy, DataPrintFunc print, void *ctx)
{
	DArray *new = NULL;
	P_VALID_CHECK_RET(data_destroy, NULL);

	new	= (DArray *)malloc(sizeof(DArray));
	P_VALID_CHECK_RET(new, NULL);
	new->size	= 0;
	new->destroy	= data_destroy;
	new->print	= print;
	if (darray_expand(new, MIN_PRE_ALLOCATE_NR) != 0) {
		SAFE_FREE(new);	
		return NULL;
	}
	return new;
}

void darray_destroy(DArray *thiz)
{
	P_VALID_CHECK_ACT(thiz, return);
	darray_foreach(thiz, thiz->destroy, NULL);
	SAFE_FREE(thiz);
	return;
}

int darray_append(DArray *thiz, void *data)
{
	P_VALID_CHECK_RET(thiz, -1);
	P_VALID_CHECK_RET(data, -1);
	return darray_insert(thiz, thiz->size, data);
}

int darray_prepend(DArray *thiz, void *data)
{
	P_VALID_CHECK_RET(thiz, -1);
	P_VALID_CHECK_RET(data, -1);
	return darray_insert(thiz, 0, data);
}

/*
 * return	: 0:invlaid 1:valid
**/
static int inline darray_index_valid(DArray *thiz, size_t i)
{
	return thiz && (i < thiz->size) ? 1 : 0; 
}

size_t inline darray_length(DArray *thiz)
{
	return thiz ? thiz->size : 0;
}

int darray_get_by_index(DArray* thiz, size_t index, void** data)
{
	P_VALID_CHECK_RET(thiz, -1);
	P_VALID_CHECK_RET(data, -1);
	if (!darray_index_valid(thiz, index))
		return -1;
	*data	= thiz->data[index];
	return 0;
}

int darray_set_by_index(DArray* thiz, size_t index, void* data)
{
	P_VALID_CHECK_RET(thiz, -1);
	P_VALID_CHECK_RET(data, -1);
	if (!darray_index_valid(thiz, index))
		return -1;
	thiz->data[index]	= data;
	return 0;
}

int darray_insert(DArray *thiz, size_t index, void *data)
{
	int ret = 0;
	size_t cursor = index;
	P_VALID_CHECK_RET(thiz, -1);

	cursor	= cursor < thiz->size ? cursor : thiz->size;
	if (darray_expand(thiz, 1) == 0) {
		size_t i = 0;
		for (i = thiz->size; i > cursor; i--)
			thiz->data[i] = thiz->data[i-1];
		thiz->data[cursor]	= data;
		thiz->size++;
		ret	= 0;
	}
	return ret;
}

static inline int darray_destroy_data(DArray *thiz, void *data)
{
	return thiz && thiz->destroy ? thiz->destroy(data, NULL) : -1;
}

int darray_delete(DArray *thiz, size_t index)
{
	size_t i = 0;
	P_VALID_CHECK_RET(thiz, -1);

	darray_destroy_data(thiz, thiz->data[index]);
	for (i = index; i + 1 < thiz->size; i++)
		thiz->data[i]	= thiz->data[i+1];
	thiz->size--;
	darray_shrink(thiz);
	return 0;
}
