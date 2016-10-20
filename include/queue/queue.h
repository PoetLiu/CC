#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct _Node
{
	int dlen;		// 数据长度
	void *data;		// 数据
	struct _Node *pNext;	// 下一个节点
	struct _Node *pPre;	// 上一个节点
}Node;

typedef struct _Queue
{
	struct _Node *front;	// 队列中第一个元素
	struct _Node *rear;	// 队列中最后一个元素
	unsigned int length;	// 队列长度
	pthread_mutex_t lock;	// 互斥锁
}Queue;

Queue *init_queue();
int empty_queue(Queue *);
int en_queue(Queue *, void *, int );
int de_queue(Queue *, void *, int *, int);
int destroy_queue(Queue *);
void travel_queue(Queue *, const char);

#define POINTER_CHECK(p, ret) 					\
	do {							\
		if (p == NULL) {				\
			QUEUE_DEBUG("NULL param: "#p"\n");	\
			return ret;				\
		}						\
	}while(0)

#define SAFE_FREE(p)			\
	do {				\
		if (p != NULL) {	\
			free(p);	\
			p	= NULL;	\
		}			\
	}while(0)

#define QUEUE_DEBUG(fmt,args...)	\
	printf("DEBUG:""%s:%s:%d "fmt, __FILE__, __FUNCTION__, __LINE__, ##args)

#endif // __QUEUE_H__
