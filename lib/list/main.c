#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
#include "debug.h"
#include "mem.h"

static int free_node(PNode node)
{
	TEST_P_NULL(node, -1);
	if (node->data)
		SAFE_FREE(node->data);
	node->dlen	= 0;
	SAFE_FREE(node);
	return 0;
}

static int print_node(const PNode const node)
{
	TEST_P_NULL(node, -1);
	TEST_P_NULL(node->data, -1);
	printf("%s\n", (char *)node->data);
	return 0;
}


static int sort_node(const PNode node_l, const PNode node_r, const int type)
{
	switch (type) {
		case DLIST_SORT_ASC:
			return (node_l->dlen > node_r->dlen) ? 1 : 0;
		case DLIST_SORT_DESC:
			return (node_l->dlen < node_r->dlen) ? 1 : 0;
		default:
			return -1;
	}		
}

static PNode get_node(const void * const data, const int dlen)
{
	PNode	node;

	node	= (PNode)malloc(sizeof(Node) * 1);
	if (node == NULL) {
		DEBUG("malloc node faild\n");
		return NULL;
	}
	memset(node, 0, sizeof(Node));

	if (data && dlen > 0) {
		node->data = (void *)malloc(sizeof(char) * dlen);
		if (node->data == NULL) {
			DEBUG("malloc node data faild\n");	
			SAFE_FREE(node);
			return NULL;
		}
		memcpy(node->data, data, dlen);
		node->dlen	= dlen;
	}
	return node;
}

static int node_sum(PNode head)
{
	int sum = 0;
	PNode node = NULL;
	
	list_for_each_node(head, node)
		sum += node->dlen;

	return sum;
}

static int lower_2_upper(PNode head)
{
	PNode node = NULL;
	int i;
	char *data;

	list_for_each_node(head, node) {
		for (i = 0, data = node->data; i < node->dlen; i++) {
			if (islower(data[i]))
				data[i] = toupper(data[i]);
		}
	}
	return 0;
}

int main(void)
{
	int i;
	Node	head;
	PNode	node	= NULL, p1 = NULL;
	char *p[]	= {
		"hello",
		"world",
		"I",
		"am",
		"chinese",
	};

	dlist_init(&head);
	
	for (i = 0; i < GET_ARRAY_SIZE(p); i++) {
		node	= get_node(p[i], strlen(p[i])+1);	
		if (node == NULL) {
			DEBUG("get_node faild\n");
			return -1;
		}
		dlist_add_tail(&head, node);
	}

	dlist_sort(&head, sort_node, DLIST_SORT_ASC);

	dlist_traverse(&head, print_node);
	DEBUG("list size %d, total len:%d\n", dlist_size(&head), node_sum(&head));

	DEBUG("deleting node dlen < 3\n");
	list_for_each_node_safe(&head, node, p1) {
		if (node->dlen < 3)
			dlist_del(node, free_node);
	} 
	DEBUG("list size %d, total len:%d\n", dlist_size(&head), node_sum(&head));

	dlist_traverse(&head, print_node);

	DEBUG("transforming to upper case\n");
	lower_2_upper(&head);
	dlist_traverse(&head, print_node);

	dlist_destory(&head, free_node);

	return 0;
}
