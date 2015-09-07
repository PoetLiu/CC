/*
 *
 * Authored by Liu Peng on: Fri Oct 31 09:25:40 CST 2014
 * -- sigdemo2.c --
 * @desc: shows how to ignore a signal
 *	  press Ctrl-\ to kill this one
 * @func:
**/

#include <stdio.h>
#include <signal.h>

int main(void)
{
	signal(SIGINT, SIG_IGN);
	printf("you can't stop me!\n");

	while (1) {
		sleep(1);
		printf("haha\n");
	}
}

