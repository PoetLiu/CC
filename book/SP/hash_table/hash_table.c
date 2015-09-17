#include "hash_table.h"
#include "list.h"
#include "mem.h"
#include <stdlib.h>

struct _HashTable 
{
	DataHashFunc hash;
	Dlist **slots;
	size_t	slot_nr;
	NODE_HANDLE data_destroy;
	void	*data_destroy_ctx;
};

HashTable *hash_table_create(NODE_HANDLE data_destroy, void *ctx, DataHashFunc hash, int slot_nr)
{
	HashTable *thiz = NULL;
	P_VALID_CHECK_RET(hash, NULL);
	if (slot_nr < 1)
		return NULL;
	thiz	= (HashTable *)malloc(sizeof(HashTable));
	if (thiz != NULL) {
		thiz->hash	= hash;	
		thiz->slot_nr	= slot_nr;
		thiz->data_destroy_ctx	= ctx;
		thiz->data_destroy	= data_destroy;
		if ((thiz->slots = (Dlist **)calloc(sizeof(Dlist*)*slot_nr, 1)) == NULL) {
			free(thiz);	
			thiz	= NULL;
		}
	}
	return thiz;
}

int hash_table_insert(HashTable *thiz, void *data)
{
	size_t index = 0;
	P_VALID_CHECK_RET(thiz, -1);
	
	index	= thiz->hash(data) % thiz->slot_nr;
	if (thiz->slots[index] == NULL)
	{
		thiz->slots[index] = dlist_create();
	}

	return dlist_prepend(thiz->slots[index], data);
}

int hash_table_delete(HashTable *thiz, NODE_FILTER filter, void *data)
{

	int index = 0;
	Dlist *dlist = NULL;
	P_VALID_CHECK_RET(thiz, -1);
	P_VALID_CHECK_RET(filter, -1);
	index	= thiz->hash(data) % thiz->slot_nr;
	dlist	= thiz->slots[index];
	if (dlist != NULL) {
		return dlist_del_by_filter(dlist, thiz->data_destroy, thiz->data_destroy_ctx, filter);
	}
	return -1;
}

Dlist *hash_table_find(HashTable *thiz, NODE_HANDLE cmp, void *data)
{
	int index = 0;
	Dlist *dlist = NULL;
	P_VALID_CHECK_RET(thiz, NULL);
	index	= thiz->hash(data) % thiz->slot_nr;
	dlist	= thiz->slots[index];
	if (dlist != NULL) {
		return dlist_find(dlist, cmp, NULL);
	}
	return NULL;
}

size_t hash_table_length(HashTable *thiz)
{
	size_t i = 0;
	size_t nr = 0;

	P_VALID_CHECK_RET(thiz, -1);
	for (i = 0; i < thiz->slot_nr; i++) {
		if (thiz->slots[i] != NULL) {
			nr	+= dlist_length(thiz->slots[i]);	
		}	
	}
	return nr;
}

int hash_table_foreach(HashTable *thiz, NODE_VISIT_HANDLE visit, void *ctx)
{
	size_t i = 0;
	P_VALID_CHECK_RET(thiz, -1);
	P_VALID_CHECK_RET(visit, -1);

	for (i = 0; i < thiz->slot_nr; i++) {
		if (thiz->slots[i] != NULL)
			dlist_foreach(thiz->slots[i], visit, ctx);
	}
	return 0;
}

void hash_table_destroy(HashTable *thiz)
{
	size_t i = 0;
	
	if (thiz != NULL) {
		for (i = 0; i < thiz->slot_nr; i++) {
			if (thiz->slots[i] != NULL) {
				dlist_destroy(thiz->slots[i], thiz->data_destroy, thiz->data_destroy_ctx);	
				thiz->slots[i]	= NULL;
			}	
		}	
		SAFE_FREE(thiz->slots);
		SAFE_FREE(thiz); 
	}
	return;
}



