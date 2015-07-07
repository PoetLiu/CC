#ifndef _LIST_H
#define _LIST_H
#include <stdio.h>

#define list_for_each_node(head, node, p)	\
	for (node = (head)->next, p = node->next; node != (head); node = p, p = p->next)

typedef struct _Node
{
	struct _Node	*next;
	struct _Node 	*prev;
	size_t		dlen;
	void		*data;
}*PNode, Node;

typedef int(*NODE_DEL_HANDLE)(PNode);
typedef int(*NODE_PRINT_HANDLE)(const PNode const);

inline PNode dlist_init(const PNode);
inline PNode dlist_add(const PNode, const PNode);
inline PNode dlist_add_tail(const PNode, const PNode);
inline int dlist_del(const PNode, NODE_DEL_HANDLE);
inline int dlist_empty(const PNode const);
inline int dlist_destory(const PNode, NODE_DEL_HANDLE);
inline int dlist_traverse(const PNode const, NODE_PRINT_HANDLE);

#endif	// _LIST_H
