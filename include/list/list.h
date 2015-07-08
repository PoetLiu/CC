#ifndef _LIST_H
#define _LIST_H
#include <stdio.h>

#define list_for_each_node(head, node)		\
	for (node = (head)->next; node != (head); node = node->next)

#define list_for_each_node_safe(head, node, p)	\
	for (node = (head)->next, p = node->next; node != (head); node = p, p = p->next)

#define list_from_start_safe(head, start, node, p)	\
	for (node = start, p = node->next; node != (head); node = p, p = p->next)

typedef struct _Node
{
	struct _Node	*next;
	struct _Node 	*prev;
	size_t		dlen;
	void		*data;
}*PNode, Node;

enum {
	DLIST_SORT_ASC, 	// 升序排列
	DLIST_SORT_DESC,	// 降序排列
}DLIST_SORT_ORDER;

typedef int(*NODE_DEL_HANDLE)(PNode);
typedef int(*NODE_SORT_HANDLE)(const PNode, const PNode, const int);
typedef int(*NODE_PRINT_HANDLE)(const PNode const);
typedef int(*NODE_VISIT_HANDLE)(void *ctx, void *data);

inline PNode dlist_init(const PNode);
inline PNode dlist_add(const PNode, const PNode);
inline PNode dlist_add_tail(const PNode, const PNode);
inline int dlist_del(const PNode, NODE_DEL_HANDLE);
inline int dlist_empty(const PNode const);
inline int dlist_destory(const PNode, NODE_DEL_HANDLE);
inline int dlist_traverse(const PNode const, NODE_PRINT_HANDLE);
inline int dlist_swap(const PNode , const PNode);
inline int dlist_size(const PNode const);
inline int dlist_sort(const PNode, const NODE_SORT_HANDLE, const int);

#endif	// _LIST_H
