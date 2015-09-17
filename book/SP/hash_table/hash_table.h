#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

struct _HashTable;
typedef struct _HashTable HashTable;
typedef int (*DataHashHandle)(HashTable *thiz, void *);
typedef int (*DataHashFunc)(HashTable *thiz);
typedef DataHashHandle DataDestroyFunc;  
typedef DataHashHandle DataCompareFunc;
typedef DataHashHandle DataVisitFunc;

#endif 
