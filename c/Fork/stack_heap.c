/*
   *
   * Authored by Liu Peng on: Mon Sep  1 13:40:14 CST 2014
   *
   * @desc: 验证子进程对父进程栈、堆空间的继承性
   *
   * @func: 子进程继承父进程的栈、堆空间，可读取这些空间的内容。但是不能修改他们
   *	    当试图修改时，采用“写时复制”，产生一个空间副本，在该副本上修改，这个
   *	    副本只对该进程有效
   */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define SAFE_FREE(p)		\
	do {			\
		free(p);	\
		p = NULL;	\
	}while(0)		\

int main(void)
{
	char 	*str = "hello, world!";
	char 	*p;
	pid_t	pid;

	p	= (char *)malloc(sizeof(char)*20);
	if (p == NULL) {
		printf("malloc error:%s\n", strerror(errno));
		return -1;
	}

	strcpy(p, str);

	if ((pid = fork()) < 0) {
		printf("fork error:%s\n", strerror(errno));
		SAFE_FREE(p);
		return -2;
	} else if ( pid == 0) {
		printf("I am in child process!\n");
		printf("str:%s\np:%s\n", str, p);
		strcpy(p, "world, hello!");
		printf("New str:%s\n", p);
	} else {
		printf("I am in father process!\n");
		printf("str:%s\np:%s\n", str, p);
	}

	SAFE_FREE(p);
	exit(0);
}

