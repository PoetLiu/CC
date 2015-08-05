/*
 *
 * Authored by Liu Peng on: Tue Jul  7 17:21:35 CST 2015
 *
 * @desc: a implement of double linked list
 *
 * @func:
**/

#include "debug.h"
#include "list.h"
#include "mem.h"
#include <stdlib.h>

struct _Node
{
	struct _Node	*next;
	struct _Node 	*prev;
	void		*data;
};

/*
 * Funcion	: check whether sort type valid
 * arguments	: type
 * return	: 0, invalid
 * 		  1, valid
**/
static inline int dlist_sort_type_valid(const int type)
{
	switch (type) {
		case DLIST_SORT_ASC:
		case DLIST_SORT_DESC:
			return 1;	// valid
		default:
			DEBUG("invalid sort type:%d\n", type);
			return 0;	// invalid
	}
}

/*
 * Funcion	: add a new node between prev and next
 * arguments	: new, new node
 * 		  prev, prev node
 * 		  next, next node
 * return	: NULL, if error
 * 		  new, if success
**/
static inline PNode __dlist_add(const PNode new, const PNode prev, const PNode next)
{
	CHECK_P_VALID(new, NULL);
	CHECK_P_VALID(prev, NULL);
	CHECK_P_VALID(next, NULL);

	prev->next	= new;
	next->prev	= new;
	new->prev	= prev;
	new->next	= next;

	return new;
}

/*
 * Funcion	: adding a new node to list which head point
 * 		  may be used for stack
 * arguments	: head, list head
 * 		  new, new node
 * return	: NULL, if err
 * 		  new, if success
 * example	: add 0, add 1, add 2, add 3.
 * 		  Then lists, head, head->next, ...
 * 		  3->2->1->0
**/
static inline PNode dlist_add(const PNode head, const PNode new)
{
	CHECK_P_VALID(head, NULL);
	CHECK_P_VALID(new, NULL);

	return __dlist_add(new, head, head->next);
}


/*
 * Funcion	: adding a new node to list which head point
 * 		  may be used for queue
 * arguments	: head, list head
 * 		  new, new node
 * return	: NULL, if err
 * 		  new, if success
 * example	: add 0, add 1, add 2, add 3.
 * 		  Then lists, head, head->next, ...
 * 		  0->1->2->3
**/
static inline PNode dlist_add_tail(const PNode head, const PNode new)
{
	CHECK_P_VALID(head, NULL);
	CHECK_P_VALID(new, NULL);

	return __dlist_add(new, head->prev, head);
}

/*
 * Funcion	: del a node beteen prev and next
 * arguments	: prev, prev node
 * 		  next, next node
 * return	: -1, if err
 * 		  0, if success
**/
static inline int __dlist_del(const PNode prev, const PNode next)
{
	CHECK_P_VALID(prev, -1);
	CHECK_P_VALID(next, -1);
	prev->next	= next;
	next->prev	= prev;
	return 0;
}

/*
 * Funcion	: del a node 
 * arguments	: del, node to del
 * 		  node_del, func pointer to handle data mem while del node
 * return	: -1, if err
 * 		  0, if success
**/
static inline int dlist_del(PNode del, NODE_HANDLE node_del)
{
	CHECK_P_VALID(del, -1);

	if(__dlist_del(del->prev, del->next))
		return -1;
	del->prev	= NULL;	
	del->next	= NULL;	

	if (!IS_NULL(node_del))
		node_del(del->data);

	SAFE_FREE(del);
	return 0;
}

static inline PNode dlist_node_new(void *data)
{
	PNode new = NULL;

	new	= (PNode)malloc(sizeof(Node));
	CHECK_P_VALID(new, NULL);
	new->data	= data;

	return new;
}

/*
 * Funcion	: swap two nodes of a dlist
 * arguments	: node_l, node left
 *		  node_r, node_right 
 * return	: -1 if error
 * 		  0 if success
**/
static inline int dlist_swap(const PNode node_l, const PNode node_r)
{
	PNode l_prev = NULL;

	CHECK_P_VALID(node_l, -1);
	CHECK_P_VALID(node_r, -1);

	l_prev = node_l->prev;
	__dlist_del(node_l->prev, node_l->next);
	dlist_add(node_r, node_l);
	__dlist_del(node_r->prev, node_r->next);
	dlist_add(l_prev, node_r);

	return 0;
}

inline PNode dlist_add_new(PNode head, void *data)
{
	PNode new = NULL;

	CHECK_P_VALID(head, NULL);
	new = dlist_node_new(data);
	CHECK_P_VALID(new, NULL);
	return dlist_add(head, new);
}

inline PNode dlist_add_tail_new(PNode head, void *data)
{
	PNode new = NULL;

	CHECK_P_VALID(head, NULL);
	new = dlist_node_new(data);
	CHECK_P_VALID(new, NULL);
	return dlist_add_tail(head, new);
}

/*
 * Funcion	: init a list head
 * return	: NULL if error
 * 		  value of head if success
**/
inline PNode dlist_init()
{
	PNode head = dlist_node_new(NULL);

	CHECK_P_VALID(head, NULL);			
	head->prev 	= head;
	head->next	= head;
	return head;
}

/*
 * Funcion	: count num of lists node
 * arguments	: head, list head
 * return	: 0, empty
 * 		  >0, not empty
**/
inline int dlist_size(const PNode const head)
{
	size_t	size = 0;
	PNode	node = NULL;

	CHECK_P_VALID(head, 0);

	if (dlist_empty(head))
		return 0;
	list_for_each_node(head, node)
		size++;

	return size;
}

/*
 * Funcion	: sort
 * arguments	: head, list head
 * 		  sorter, check whether two node need to swap
 * 		  	  USER MUST IMPEMENT !
 * 		  type, sort order
 * return	: -1, if error
 * 		  0, if sucess
**/
inline int dlist_sort(const PNode head, const NODE_SORT_HANDLE sorter, const int type)
{
	PNode node_r = NULL, node_l = NULL;
	PNode p	= NULL, p1 = NULL;

	CHECK_P_VALID(head, -1);
	CHECK_P_VALID(sorter, -1);
	if (!dlist_sort_type_valid(type))	
		return -1;

	list_for_each_node_safe(head, node_l, p) {
		list_from_start_safe(head, p, node_r, p1) {
			if (sorter(node_l->data, node_r->data, type))
				dlist_swap(node_l, node_r);
		}
	}

	return 0;
}

/*
 * Funcion	: user API, whether list is empty
 * arguments	: head, list head
 * return	: 1, if empty
 * 		  0, if not empty 
**/
inline int dlist_empty(const PNode const head)
{
	CHECK_P_VALID(head, 1);
	return  head->next == head;
}

/*
 * Funcion	: user API, destroy lits 
 * arguments	: head, list head
 * 		  node_del, func pointer to handle data mem while del node
 * return	: -1, if err
 * 		  0, if success
**/
inline int dlist_destory(const PNode head, const NODE_HANDLE node_del)
{
	PNode next = NULL, del = NULL;

	CHECK_P_VALID(head, -1);

	if (dlist_empty(head))
		return 0;

	list_for_each_node_safe(head, del, next) {
		dlist_del(del, node_del);
	}
	return 0;
}

inline int dlist_del_by_filter(PNode head, NODE_HANDLE node_del, NODE_HANDLE filter)
{
	PNode next = NULL, del = NULL;

	CHECK_P_VALID(head, -1);
	CHECK_P_VALID(node_del, -1);

	if (dlist_empty(head))
		return 0;

	list_for_each_node_safe(head, del, next) {
		if (filter(del->data))
			dlist_del(del, node_del);
	}
	return 0;

}

/*
 * Funcion	: user API, traverse a list 
 * arguments	: head, list head
 * 		  node_print, func pointer to print data
 * return	: -1, if err
 * 		  0, if success
**/
inline int dlist_foreach(const PNode const head, NODE_VISIT_HANDLE visit, void *ctx)
{
	PNode node = NULL, next = NULL;

	CHECK_P_VALID(head, -1);
	CHECK_P_VALID(visit, -1);

	if (dlist_empty(head)) {
		printf("empty list\n");
		return 0;
	}

	list_for_each_node_safe(head, node, next) {
		visit(node->data, ctx);
	}

	return 0;
}

inline int dlist_find_max(const PNode const head, NODE_VISIT_HANDLE visit, void *ctx)
{
	return dlist_foreach(head, visit, ctx);
}

inline int dlist_sum(const PNode const head, NODE_VISIT_HANDLE visit, void *ctx)
{
	memset(ctx, 0, sizeof(int));
	return dlist_foreach(head, visit, ctx);
}

inline int dlist_print(const PNode const head, NODE_VISIT_HANDLE visit)
{
	return dlist_foreach(head, visit, NULL);
}
