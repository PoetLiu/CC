/*
 *
 * Authored by Liu Peng on: Fri Oct 31 17:06:42 CST 2014
 *
 * @desc: shows use of sigaction()
 *	  blocks Ctrl+C handler
 *	  does not reset Ctrl+C handler, so two kill
 * @func:
**/

#include <stdio.h>
#include <signal.h>

#define INPUTLEN	100

void inthandler();

int main(void)
{
	struct sigaction	newhandler;	/* new settings */
	sigset_t		blocked;	/* set of blocked sigs */
	char			x[INPUTLEN];

	/* load these two members first */
	newhandler.sa_handler	= inthandler; 	/* handler function */
	newhandler.sa_flags	= SA_RESETHAND | SA_RESTART;	/* options */

	/* then build the list of blockd signals */
	sigemptyset(&blocked);			/* clear all bits */
	sigaddset(&blocked, SIGQUIT);		/* add SIGQUIT to list */
	newhandler.sa_mask	= blocked;	/* store blockmask */

	if (sigaction(SIGINT, &newhandler, NULL) == -1)
		perror("sigaction");
	else
		while (1) {
			fgets(x, INPUTLEN, stdin);
			printf("input:%s", x);
		}
	
}

void inthandler(int s)
{
	printf("Called with signal %d\n", s);
	sleep(s);
	printf("done handling signal %d\n", s);
}
