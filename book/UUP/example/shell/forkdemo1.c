/*
 *
 * Authored by Liu Peng on: Mon Nov  3 08:58:17 CST 2014
 *
 * @desc: shows how fork creates two processes, distinguishable
 *	  by the different return values from fork()
 * @func:
**/

#include <stdio.h>

int main(void)
{
	int	ret_from_fork, mypid;

	mypid	= getpid();			/* who am i ? */
	printf("Before:my pid is %d\n", mypid);	/* tell the world */

	ret_from_fork	= fork();

	sleep(1);
	printf("After:my pid is %d, fork() said %d\n", getpid(), ret_from_fork);
	return 0;
}
