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

typedef int(*NODE_DEL_HANDLE)(PNode node);
typedef int(*NODE_PRINT_HANDLE)(PNode node);

inline PNode dlist_init(PNode head);
inline PNode dlist_add(PNode head, PNode new);
inline PNode dlist_add_tail(PNode head, PNode new);
inline int dlist_del(PNode del, NODE_DEL_HANDLE node_del);
inline int dlist_empty(const PNode head);
inline int dlist_destory(PNode head, NODE_DEL_HANDLE node_del);
inline int dlist_traverse(const PNode head, NODE_PRINT_HANDLE node_print);

#endif	// _LIST_H
