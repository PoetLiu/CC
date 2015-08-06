#ifndef _LIST_H
#define _LIST_H
#include <stdio.h>

#define list_for_each_node(head, node)		\
	for (node = (head)->next; node != (head); node = node->next)

#define list_for_each_node_safe(head, node, p)	\
	for (node = (head)->next, p = node->next; node != (head); node = p, p = p->next)

#define list_from_start_safe(head, start, node, p)	\
	for (node = start, p = node->next; node != (head); node = p, p = p->next)


struct _Node;
typedef struct _Node* PNode;
typedef struct _Node Node;

enum {
	DLIST_SORT_ASC, 	// 升序排列
	DLIST_SORT_DESC,	// 降序排列
}DLIST_SORT_ORDER;

typedef int(*NODE_HANDLE)(void *);
typedef int(*NODE_SORT_HANDLE)(void *, void *, const int);
typedef int(*NODE_VISIT_HANDLE)(void *, void *);

inline PNode dlist_init(void);
inline PNode dlist_add_new(const PNode, void *data);
inline PNode dlist_add_tail_new(const PNode, void *data);
inline int dlist_empty(const PNode const);
inline int dlist_destory(const PNode, NODE_HANDLE);
inline int dlist_size(const PNode const);
inline int dlist_sort(const PNode, const NODE_SORT_HANDLE, const int);
inline int dlist_find_max(const PNode const, NODE_VISIT_HANDLE, void *);
inline int dlist_sum(const PNode const, NODE_VISIT_HANDLE, void *);
inline int dlist_print(const PNode const, NODE_VISIT_HANDLE);
inline int dlist_foreach(const PNode const, NODE_VISIT_HANDLE, void *);
inline int dlist_del_by_filter(PNode, NODE_HANDLE, NODE_HANDLE);

#endif	// _LIST_H
