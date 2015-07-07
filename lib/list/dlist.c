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

/*
 * Funcion	: init a list head
 * arguments	: PNode, a pointer to struct which already have mem space
 * return	: NULL if error
 * 		  value of head if success
**/
inline PNode dlist_init(const PNode head)
{
	TEST_P_NULL(head, NULL);			
	head->prev 	= head;
	head->next	= head;
	return head;
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
	TEST_P_NULL(new, NULL);
	TEST_P_NULL(prev, NULL);
	TEST_P_NULL(next, NULL);

	prev->next	= new;
	next->prev	= new;
	new->prev	= prev;
	new->next	= next;

	return new;
}

/*
 * Funcion	: user API, adding a new node to list which head point
 * 		  may be used for stack
 * arguments	: head, list head
 * 		  new, new node
 * return	: NULL, if err
 * 		  new, if success
**/
inline PNode dlist_add(const PNode head, const PNode new)
{
	TEST_P_NULL(head, NULL);
	TEST_P_NULL(new, NULL);

	return __dlist_add(new, head, head->next);
}


/*
 * Funcion	: user API, adding a new node to list which head point
 * 		  may be used for queue
 * arguments	: head, list head
 * 		  new, new node
 * return	: NULL, if err
 * 		  new, if success
**/
inline PNode dlist_add_tail(const PNode head, const PNode new)
{
	TEST_P_NULL(head, NULL);
	TEST_P_NULL(new, NULL);

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
	TEST_P_NULL(prev, -1);
	TEST_P_NULL(next, -1);
	prev->next	= next;
	next->prev	= prev;
	return 0;
}

/*
 * Funcion	: user API, del a node 
 * arguments	: del, node to del
 * 		  node_del, func pointer to handle data mem while del node
 * return	: -1, if err
 * 		  0, if success
**/
inline int dlist_del(const PNode del, NODE_DEL_HANDLE node_del)
{
	TEST_P_NULL(del, -1);

	if(__dlist_del(del->prev, del->next))
		return -1;
	del->prev	= NULL;	
	del->next	= NULL;	

	if (!IS_NULL(node_del))
		node_del(del);
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
	TEST_P_NULL(head, 1);
	return  head->next == head;
}

/*
 * Funcion	: user API, destroy lits 
 * arguments	: head, list head
 * 		  node_del, func pointer to handle data mem while del node
 * return	: -1, if err
 * 		  0, if success
**/
inline int dlist_destory(const PNode head, NODE_DEL_HANDLE node_del)
{
	PNode next = head->next;
	PNode del = next;

	TEST_P_NULL(head, -1);

	if (dlist_empty(head))
		return 0;

	while (del != head) {
		next	= next->next;
		dlist_del(del, node_del);
		del	= next;
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
inline int dlist_traverse(const PNode const head, NODE_PRINT_HANDLE node_print)
{
	PNode node = head->next;

	TEST_P_NULL(head, -1);
	TEST_P_NULL(node_print, -1);

	if (dlist_empty(head)) {
		printf("empty list\n");
		return 0;
	}

	while (node != head) {
		node_print(node);	
		node	= node->next;
	}

	return 0;
}

// a example of node_del
int __free_node(PNode node)
{
	TEST_P_NULL(node, -1);
	if (node->data)
		SAFE_FREE(node->data);
	node->dlen	= 0;
	SAFE_FREE(node);
	return 0;
}

// a example of node_print
int __print_node(const PNode const node)
{
	TEST_P_NULL(node, -1);
	TEST_P_NULL(node->data, -1);
	printf("%s\n", (char *)node->data);
	return 0;
}
