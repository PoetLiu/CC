/*
 *
 * Authored by Liu Peng on: Fri Oct 31 14:11:20 CST 2014
 *
 * @desc: demonstrates use of interval timer to generate regular
 *	  signals, which are in turn caught and used to count down
 * @func:
**/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>

void countdown(int);
int set_ticker(int n_msecs);

int main(void)
{
	signal(SIGALRM, countdown);

	if (set_ticker(1000) == -1)
		perror("set_ticker");
	else
		while(1)
			pause();
	return 0;
}

void countdown(int signum)
{
	static int num = 10;
	printf("%d..", num--);
	fflush(stdout);

	if (num < 0) {
		printf("DONE!\n");
		exit(0);
	}
}

/*
 * Funcion	: set_ticker(number_of_milliseconds)
 *		  arranges for interval timer to issue SIGALRMs at regular intervals
 * Author	:
 *
 * arguments	: arg in milliseconds, converted into whole seconds and microseconds
 * return	: -1 on error, 0 for ok
**/
int set_ticker(int n_msecs)
{
	struct itimerval new_timeset;
	long n_sec, n_usecs;

	n_sec	= n_msecs / 1000;			/* int part */
	n_usecs	= (n_msecs % 1000) * 1000L;		/* remainder */
	
	new_timeset.it_interval.tv_sec	= n_sec;	/* set reload */
	new_timeset.it_interval.tv_usec	= n_usecs;	/* new ticker walue */

	new_timeset.it_value.tv_sec	= n_sec;	/* store this */
	new_timeset.it_value.tv_usec	= n_usecs;	/* and this */

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}
