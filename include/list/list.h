#ifndef _LIST_H
#define _LIST_H
#include <stdio.h>

typedef struct _Node
{
	struct _Node	*next;
	struct _Node 	*prev;
	size_t		dlen;
	void		*data;
}*PNode, Node;

typedef int(*NODE_DEL_HANDLE)(PNode node);
typedef int(*NODE_PRINT_HANDLE)(PNode node);

inline PNode list_init(PNode head);
inline PNode list_add(PNode head, PNode new);
inline PNode list_add_tail(PNode head, PNode new);
inline int list_del(PNode del, NODE_DEL_HANDLE node_del);
inline int list_empty(PNode head);
inline int list_destory(PNode head, NODE_DEL_HANDLE node_del);
inline int list_traverse(const PNode head, NODE_PRINT_HANDLE node_print);

#define list_for_each_node(head, node, p)	\
	for (node = (head)->next, p = node->next; node != (head); node = p, p = p->next)

#endif	// _LIST_H
