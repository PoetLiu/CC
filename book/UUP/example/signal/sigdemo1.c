/*
 *
 * Authored by Liu Peng on: Fri Oct 31 09:20:49 CST 2014
 * --sigdemo1.c--
 * @desc: shows how a signal handler works
 *	  run this and press Ctrl-C a few times
 * @func:
**/
#include <stdio.h>
#include <signal.h>

int main(void)
{
	void f(int);
	int i;

	signal(SIGINT, f);

	for (i = 0; i < 5; i++) {
		printf("hello\n");
		sleep(1);
	}
	return 0;
}

/*
 * Funcion	: SIGINT signal handler
 *
 * Author	:
 *
 * arguments	:
 * return	:
**/
void f(int signum)
{
	printf("OUCH!\n");
}
