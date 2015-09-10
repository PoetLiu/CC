#ifndef _D_ARRAY_H_
#define _D_ARRAY_H_

struct _DArray;
typedef struct _DArray DArray;
typedef int (*DataVisitFunc)(void *data, void *ctx);
typedef DataVisitFunc DataDestroyFunc;
typedef DataVisitFunc DataCompareFunc;
typedef DataVisitFunc DataPrintFunc;

DArray* darray_create(DataDestroyFunc data_destroy, DataPrintFunc print, void* ctx);
int darray_insert(DArray* thiz, size_t index, void* data);
int darray_prepend(DArray* thiz, void* data);
int darray_append(DArray* thiz, void* data);
int darray_delete(DArray* thiz, size_t index);
int darray_get_by_index(DArray* thiz, size_t index, void** data);
int darray_set_by_index(DArray* thiz, size_t index, void* data);
size_t darray_length(DArray* thiz);
int darray_find(DArray* thiz, DataCompareFunc cmp, void* ctx);
int darray_foreach(DArray* thiz, DataVisitFunc visit, void* ctx);
int darray_print(DArray *thiz);
void darray_destroy(DArray* thiz);
#endif


