/*
   *
   * Authored by Liu Peng on: Wed Aug 27 15:48:38 CST 2014
   *
   * @desc: 捕捉SIGINT, SIGQUTI信号
   *
   * @func:
   */
#include <stdio.h>
#include <errno.h>
#include <signal.h>

volatile sig_atomic_t	quitflag; /* set nonzero by signal handler */
static void sig_int(int signo);

int main(void)
{

	sigset_t	newmask, oldmask, zeromask;

	if (signal(SIGINT, sig_int) == SIG_ERR)
		printf("signal(SIGINT) error\n");
	if (signal(SIGQUIT, sig_int) == SIG_ERR)
		printf("signal(SIGQUIT) error\n");

	sigemptyset(&zeromask);
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGQUIT);
	/*
	 * Block SIGQUTI and save current signal mask.
	 */
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
		printf("SIG_BLOCK error\n");
	
	while (quitflag == 0)
		sigsuspend(&zeromask);

	/*
	 * SIGQUIT has been caught and is now blocked; do whatever.
	 */
	quitflag	= 0;

	/*
	 * Reset signal mask which unblocks SIGQUIT.
	 */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
		printf("SIG_SETMASK error\n");
	return 0;
}

/*
   * Funcion	: One signal handler for SIGINT and SIGQUIT
   * Author	: Liu Peng
   * arguments	:
   * return	:
   */

static void sig_int(int signo)
{
	if (signo == SIGINT)
		printf("\ninterrupt\n"); 
	else if (signo == SIGQUIT)
		quitflag	= 1; /* set flag for main loop */
}

