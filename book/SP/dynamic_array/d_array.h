#ifndef _D_ARRAY_H_
#define _D_ARRAY_H_

struct _DArray;
typedef struct _DArray DArray;
typedef int (*DataVisitFunc)(void *data, void *ctx);
typedef DataVisitFunc DataDestroyFunc;
typedef DataVisitFunc DataPrintFunc;

/* 返回值为数据比较结果，enum CMP_RESULT. 
 * */
typedef int (*DataCompareFunc)(void *ldata, void *rdata);

DArray* darray_create(DataDestroyFunc data_destroy, DataCompareFunc cmp, DataPrintFunc print, void* ctx);
int darray_insert(DArray* thiz, size_t index, void* data);
int darray_prepend(DArray* thiz, void* data);
int darray_append(DArray* thiz, void* data);
int darray_delete(DArray* thiz, size_t index);
int darray_get_by_index(DArray* thiz, size_t index, void** data);
int darray_set_by_index(DArray* thiz, size_t index, void* data);
size_t darray_length(DArray* thiz);
int darray_find(DArray* thiz, void* data);
int darray_foreach(DArray* thiz, DataVisitFunc visit, void* ctx);
int darray_print(DArray *thiz);
int darray_unique_print(DArray *thiz);
void darray_destroy(DArray* thiz);
int darray_sort(DArray *thiz, int type, int method);

enum SORT_TYPE {
	SORT_TYPE_ASC,	// 升序
	SORT_TYPE_DESC,	// 降序
};

enum SORT_METHOD {
	SORT_METHOD_QK,		// 快速排序
	SORT_METHOD_BUBBLE,	// 冒泡排序
	SORT_METHOD_MERGE,	// 归并排序
};

enum CMP_RESULT {
	RES_EQUAL,	// 等于
	RES_GTR,	// 大于
	RES_LT,		// 小于
};

#endif


