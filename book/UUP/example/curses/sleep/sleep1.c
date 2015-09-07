/*
 *
 * Authored by Liu Peng on: Fri Oct 31 13:54:46 CST 2014
 *
 * @desc	: show how sleep works
 * 		  sleep1
 * @outline	: sets handler, set alarm, pauses, then returns
**/

#include <stdio.h>
#include <signal.h>
//#define SHHHH

int main(void)
{
	void wakeup(int);

	printf("about to sleep for 4 seconds\n");
	signal(SIGALRM, wakeup);	/* cauch it */
	alarm(4);			/* set clock */
	pause();			/* freeze here */
	printf("Morning so soon ?\n");	/* back to work */
}

void wakeup(int signum)
{
#ifndef SHHHH
	printf("Alarm received from kernel\n");
#endif
}
