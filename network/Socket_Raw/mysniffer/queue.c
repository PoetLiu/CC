#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <pthread.h>
#include "queue.h"

/*--------------------------------------------
 * Node1.pNext->Node2.pNext->Node3.pNext->NULL
 *   |	 		       | 		
 *  rear 		     front
 *--------------------------------------------
*/

Queue *init_queue()
{
	Queue *new_q = NULL;
	new_q	= (Queue *)malloc(sizeof(Queue));
	POINTER_CHECK(new_q, NULL);

	new_q->front	= NULL;
	new_q->rear	= NULL;
	new_q->length	= 0;
	pthread_mutex_init(&new_q->lock, NULL);

	return new_q;
}

int empty_queue(Queue *pQueue)
{
	unsigned int len = 0;

	POINTER_CHECK(pQueue, 1);
	pthread_mutex_lock(&pQueue->lock);
	len	= pQueue->length;
	pthread_mutex_unlock(&pQueue->lock);

	return len == 0;
}

// 入队， 从队列末尾添加一个节点
int en_queue(Queue *pQueue, void *data, int dlen)
{
	Node *new_node = NULL;
	// param check
	POINTER_CHECK(pQueue, -1);
	POINTER_CHECK(data, -1);

	// get new_node
	new_node	= (Node *)malloc(sizeof(Node));
	POINTER_CHECK(new_node, -1);
	new_node->data	= (void *)malloc(dlen);
	if (new_node->data == NULL) {
		SAFE_FREE(new_node);
		return -1;
	}
	memcpy(new_node->data, data, dlen);
	new_node->dlen	= dlen;
	new_node->pPre	= NULL;
	new_node->pNext	= NULL;

	pthread_mutex_lock(&pQueue->lock);
	// add to queue
	if (!pQueue->rear) {
		pQueue->rear	= new_node;
	} else {
		new_node->pNext		= pQueue->rear;
		pQueue->rear->pPre	= new_node;
		pQueue->rear		= new_node;
	}

	if (!pQueue->front)
		pQueue->front	= new_node;

	pQueue->length++;
	pthread_mutex_unlock(&pQueue->lock);

	return 0;
}

static int free_node(Node *node)
{
	POINTER_CHECK(node, -1);
	SAFE_FREE(node->data);
	SAFE_FREE(node);
	return 0;
}

// 如果copy 值为1， Node出队时， 其Node->data将会被拷贝至data实参
// 调用者应该保证data所指向的空间足够大
// 如果copy 值为0， NOde出队时， 仅仅复制Node->data的指针给data实参
// 调用者应该负责data的内存释放
int de_queue(Queue *pQueue, void *data, int *dlen, int cp_flag)
{
	Node *de_node = NULL;
	int ret;
	POINTER_CHECK(pQueue, -1);
	
	if (empty_queue(pQueue))
		return -1;

	pthread_mutex_lock(&pQueue->lock);
	de_node	= pQueue->front;

	if (data) {
		if (cp_flag) {
			memcpy(data, de_node->data, de_node->dlen);
		} else {
			*(void **)data	= de_node->data;
			de_node->data	= NULL;
		}
	}
	if (dlen)
		*dlen	= de_node->dlen;
	if (de_node->pPre)
		de_node->pPre->pNext = NULL;
	if (pQueue->rear == de_node)
		pQueue->rear	= NULL;

	pQueue->front	= de_node->pPre;
	pQueue->length--;
	ret	= free_node(de_node);
	pthread_mutex_unlock(&pQueue->lock);

	return ret;
}

static void print_data(void *buf, int blen, const char fmt)
{
	int i;

	switch (fmt) {
		case 'd':
			for (i = 0; i < blen; i+=sizeof(int))
				printf("%d ", *((int *)((char *)buf + i)));
			break;
		case 's':
			printf("%s", (char *)buf);
			break;
		default:
			break;
	}
	printf(" len:%d\n", blen);
}

void travel_queue(Queue *pQueue, const char fmt)
{

	Node *p_node = NULL;
	printf("Queue:\n");
	if (empty_queue(pQueue))
		printf("empty\n");

	pthread_mutex_lock(&pQueue->lock);
	p_node	= pQueue->front;
	while (p_node) {
		print_data(p_node->data, p_node->dlen, fmt);		
		p_node	= p_node->pPre;	
	}
	pthread_mutex_unlock(&pQueue->lock);
}

int destroy_queue(Queue *pQueue)
{
	Node *de_node = NULL, *de_node_pre = NULL;
	POINTER_CHECK(pQueue, -1);
	if (empty_queue(pQueue)) 
		goto free_out;

	pthread_mutex_lock(&pQueue->lock);
	de_node	= pQueue->front;
	while (de_node) {
		de_node_pre	= de_node->pPre ? de_node->pPre : NULL;
		free_node(de_node);
		de_node		= de_node_pre;
	}
	pthread_mutex_unlock(&pQueue->lock);

free_out:
	pthread_mutex_destroy(&pQueue->lock);
	SAFE_FREE(pQueue);
	return 0;
}

