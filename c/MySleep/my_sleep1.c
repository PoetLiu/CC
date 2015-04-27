/*
   *
   * Authored by Liu Peng on: Wed Aug 27 09:31:55 CST 2014
   *
   * @desc: 使用signal、alarm、pause函数实现sleep函数延时功能
   *	    这种简单的实现存在3个问题，详见apue 10.10章节
   * @func: 延时nsecs秒
   */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static void sig_alrm(int signo);
unsigned int sleep1(unsigned int nsecs);

int main(void)
{
	int i;

	for (i = 0; i < 10; i++) {
		printf("hello, world %d\n", i);
		sleep1(1);
	}
	return 0;
}

static void sig_alrm(int signo)
{
	/* nothing to do , just return to wake up the pause*/
}

/*
   * Funcion	: 延时nsecs秒
   * Author	: Liu Peng
   * arguments	: 
   * return	: 上次设置闹钟时未超时剩余的时间值
   */

unsigned int sleep1(unsigned int nsecs)
{
	if (signal(SIGALRM, sig_alrm) == SIG_ERR)
		return(nsecs);
	alarm(nsecs);		/* start the timer */
	pause();		/* next caught signal wakes us up */
	return(alarm(0));	/* turn off timer, return unslept time */
}

