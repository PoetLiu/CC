#include <stdlib.h>
#include "mem.h"
#include "debug.h"
#include "d_array.h"

#define MIN_PRE_ALLOCATE_NR	10
struct _DArray {
	size_t size;
	size_t alloc_size;
	DataDestroyFunc destroy;
	DataCompareFunc cmp;
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
	int i = 0, is_end = 0;

	P_VALID_CHECK_RET(thiz, -1);
	P_VALID_CHECK_RET(thiz->print, -1);

	for (i = 0; i < thiz->size; i++) { 
		is_end	= (i == (thiz->size - 1));
		thiz->print(thiz->data[i], &is_end);
	}
	
	return 0;
}

int darray_unique_print(DArray *thiz)
{
	void *ctx = NULL;
	int i = 0, is_end = 0;

	P_VALID_CHECK_RET(thiz, -1);
	P_VALID_CHECK_RET(thiz->print, -1);
	P_VALID_CHECK_RET(thiz->cmp, -1);
	ctx	= thiz->data[0];
	for (i = 0; i < thiz->size; i++) { 
		if ((thiz->cmp(thiz->data[i], ctx) == RES_EQUAL) && i != 0) 
			continue;
		ctx	= thiz->data[i];
		is_end	= (i == thiz->size - 1 ? 1 : 0);
		thiz->print(thiz->data[i], &is_end);
	}
	
	return 0;
}

int darray_find(DArray* thiz, void* data)
{
	int i = 0;
	P_VALID_CHECK_RET(thiz, -1);
	P_VALID_CHECK_RET(data, -1);

	for (i = 0; i < thiz->size; i++)
		if (thiz->data[i] == data)
			return i;
	return -1;
}

DArray* darray_create(DataDestroyFunc data_destroy, DataCompareFunc cmp, 
		DataPrintFunc print, void *ctx)
{
	DArray *new = NULL;
	P_VALID_CHECK_RET(data_destroy, NULL);

	new	= (DArray *)malloc(sizeof(DArray));
	P_VALID_CHECK_RET(new, NULL);
	new->size	= 0;
	new->destroy	= data_destroy;
	new->print	= print;
	new->cmp	= cmp;
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

int darray_swap(void **ldata, void **rdata)
{
	void *tmp = NULL;
	P_VALID_CHECK_RET(ldata, -1);
	P_VALID_CHECK_RET(rdata, -1);
	tmp	= *ldata;
	*ldata	= *rdata;
	*rdata	= tmp;
	return 0;
}

static int darray_sort_bubble(DArray *thiz, DataCompareFunc cmp, int type)
{
	int i = 0, j = 0;
	for (i = 0; i < thiz->size - 1; i++)
		for (j = i + 1; j < thiz->size; j++)
			if ((type == SORT_TYPE_ASC && (cmp(thiz->data[i], thiz->data[j]) == RES_GTR))
				|| (type == SORT_TYPE_DESC && (cmp(thiz->data[i], thiz->data[j]) == RES_LT)))
				darray_swap(&thiz->data[i], &thiz->data[j]);
	return 0;
}

static void _darray_sort_quick(void **data, size_t left, size_t right, DataCompareFunc cmp,  int type)
{
	P_VALID_CHECK_ACT(data, return);
	P_VALID_CHECK_ACT(cmp, return);
	void *key = data[left];
	size_t save_left = left;
	size_t save_right = right;
	
	while (left < right) {
		while ((((cmp(data[right], key) != RES_LT) && type == SORT_TYPE_ASC) ||
			((cmp(data[right], key) != RES_GTR) && type == SORT_TYPE_DESC)) && left < right) right--;
		if (left != right)
			data[left++]	= data[right];
		while ((((cmp(data[left], key) != RES_GTR) && type == SORT_TYPE_ASC) ||
			((cmp(data[left], key) != RES_LT) && type == SORT_TYPE_DESC)) && left < right) left++;
		if (left != right)
			data[right--]	= data[left];
	}

	data[left]	= key;
	if (save_left < left)
		_darray_sort_quick(data, save_left, left - 1, cmp, type);
	if (save_right > left)
		_darray_sort_quick(data, left + 1, save_right, cmp, type);

	return;
}

static int darray_sort_quick(DArray *thiz, DataCompareFunc cmp, int type)
{
	P_VALID_CHECK_RET(thiz, -1);
	P_VALID_CHECK_RET(cmp, -1);
	if (thiz->size > 1)
		_darray_sort_quick(thiz->data, 0, thiz->size - 1, cmp, type);
	return 0;
}

int darray_sort(DArray *thiz, int type, int method)
{
	P_VALID_CHECK_RET(thiz, -1);
	P_VALID_CHECK_RET(thiz->cmp, -1);

	switch (method) {
		case SORT_METHOD_QK:
			return darray_sort_quick(thiz, thiz->cmp, type);
			break;
		case SORT_METHOD_BUBBLE:
			return darray_sort_bubble(thiz, thiz->cmp, type);
		case SORT_METHOD_MERGE:
			break;
		default:
			DEBUG("unkown method:%d\n", method);
			return -1;
	}
	return 0;
}

int darray_qsearch(DArray *thiz, void *data)
{
	int start = 0, end = 0, mid = 0, result = 0;

	P_VALID_CHECK_RET(thiz, -1);
	P_VALID_CHECK_RET(thiz->cmp, -1);
	P_VALID_CHECK_RET(data, -1);

	if (thiz->size == 0)
		return -1;

	end	= thiz->size - 1;
	while (start <= end) {
		mid	= start + ((end - start) >> 1);
		result	= thiz->cmp(data, thiz->data[mid]);

		if (result == RES_GTR) 
			start	= mid + 1;
		else if (result == RES_LT)
			end	= mid - 1;
		else
			return mid;
	}

	return -1;
}

