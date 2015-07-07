#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

static int print_node(PNode node)
{
	TEST_P_NULL(node, -1);
	TEST_P_NULL(node->data, -1);
	printf("%s\n", (char *)node->data);
	return 0;
}

static PNode get_node(void *data, int dlen)
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

	dlist_traverse(&head, print_node);

	list_for_each_node(&head, node, p1) {
		if (node->dlen < 3)
			dlist_del(node, free_node);
	}

	dlist_traverse(&head, print_node);
	dlist_destory(&head, free_node);

	return 0;
}
