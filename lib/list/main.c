#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
#include "debug.h"
#include "mem.h"

struct node_data {
	size_t len;
	char *buf;
};

static int free_data(void *data)
{
	SAFE_FREE(data);
	return 0;
}

static int data_filter(void *data)
{
	struct node_data *d = (struct node_data *)data;

	CHECK_P_VALID(d, 0);
	if (d->len < 3)
		return 1;
	return 0;
}

static int print_data(void *data, void *ctx)
{
	struct node_data *d = (struct node_data *)data;

	CHECK_P_VALID(d, -1);
	CHECK_P_VALID(d->buf, -1);

	printf("%s\n", (char *)d->buf);
	return 0;
}

static int sort_node(void *data_l, void *data_r, const int type)
{
	struct node_data *dl = (struct node_data *)data_l;
	struct node_data *dr = (struct node_data *)data_r;

	CHECK_P_VALID(data_l, -1);
	CHECK_P_VALID(data_r, -1);

	switch (type) {
		case DLIST_SORT_ASC:
			return (dl->len > dr->len) ? 1 : 0;
		case DLIST_SORT_DESC:
			return (dl->len < dr->len) ? 1 : 0;
		default:
			return -1;
	}		
}

static struct node_data* node_data_new(const void * const data, const int len)
{
	struct node_data *new = NULL;

	new	= (struct node_data *)malloc(sizeof(struct node_data) * 1);
	if (new	== NULL) {
		DEBUG("malloc node faild\n");
		return NULL;
	}
	memset(new, 0, sizeof(struct node_data));

	if (data && len > 0) {
		new->buf	= (void *)malloc(sizeof(char) * len);
		if (new->buf == NULL) {
			DEBUG("malloc node data faild\n");	
			SAFE_FREE(new);
			return NULL;
		}
		memcpy(new->buf, data, len);
		new->len	= len;
	}
	return new;
}

static int node_sum(void *data, void *ctx)
{
	int *sum = (int *)ctx;
	struct node_data *d = (struct node_data *)data;

	CHECK_P_VALID(d, -1);
	CHECK_P_VALID(sum, -1);
	*sum	+= d->len;
	return *sum;
}

static int node_max(void *data, void *ctx)
{
	int *max = (int *)ctx;
	struct node_data *d = (struct node_data *)data;

	CHECK_P_VALID(d, -1);
	CHECK_P_VALID(max, -1);
	*max	= (d->len > *max) ? d->len : *max;
	return *max;
}

static int lower_2_upper(void *data, void *ctx)
{
	int i;
	struct node_data *d = (struct node_data *)data;

	CHECK_P_VALID(d, -1);
	CHECK_P_VALID(d->buf, -1);

	for (i = 0; i < d->len; i++) {
		if (islower(d->buf[i]))
			d->buf[i] = toupper(d->buf[i]);
	}
	return 0;
}

int main(void)
{
	int i, sum = 0, max = 0;
	PNode	head;
	struct node_data *data = NULL;
	char *p[]	= {
		"hello",
		"world",
		"I",
		"am",
		"chinese",
	};

	head	= dlist_init();
	
	for (i = 0; i < GET_ARRAY_SIZE(p); i++) {
		data	= node_data_new(p[i], strlen(p[i])+1);	
		if (data == NULL) {
			DEBUG("get_node faild\n");
			return -1;
		}
		dlist_add_tail_new(head, data);
	}

	dlist_sort(head, sort_node, DLIST_SORT_ASC);

	dlist_print(head, print_data); 
	dlist_sum(head, node_sum, &sum);
	dlist_find_max(head, node_max, &max); 
	DEBUG("list size %d, total len:%d, max len:%d\n", dlist_size(head), sum, max);

	DEBUG("deleting node len < 3\n");
	dlist_del_by_filter(head, free_data, data_filter);
	dlist_sum(head, node_sum, &sum);
	DEBUG("list size %d, total len:%d\n", dlist_size(head), sum);

	dlist_print(head, print_data);

	DEBUG("transforming to upper case\n");
	dlist_foreach(head, lower_2_upper, NULL);
	dlist_print(head, print_data);

	dlist_destory(head, free_data);

	return 0;
}
