#include <stdio.h>
#include <signal.h>
#include <errno.h>

static void sig_int(int);
static void pr_mask(const char *str);

int main(void)
{
	sigset_t	newmask, oldmask, waitmask;

	pr_mask("program start: ");

	if (signal(SIGINT, sig_int) == SIG_ERR)
		printf("signal(SIGINT) error\n");

	sigemptyset(&waitmask);
	sigaddset(&waitmask, SIGUSR1);
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGINT);

	/*
	 * Blcck SIGINT and save current signal mask.
	 */

	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
		printf("SIG_BLOCK error");

	/*
	 * Critical region of code.
	 */
	pr_mask("in critical region: ");

	/*
	 * Pause, allowing all signals except SIGUSR1.
	 */
	if (sigsuspend(&waitmask) != -1)
		printf("sigsuspend error\n");

	pr_mask("after return from sigsuspend: ");

	/*
	 * Reset signal mask which unblocks SIGINT.
	 */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
		printf("SIS_SETMASK error\n");

	/*
	 * And continiu processing ...
	 */
	pr_mask("program exit: ");
	return 0;
}

static void sig_int(int signo)
{
	pr_mask("\nin sig_int :");
}

static void pr_mask(const char *str)
{
	sigset_t	sigset;
	int		errno_save;

	errno_save	= errno;
	if (sigprocmask(0, NULL, &sigset) < 0)
		printf("sigprocmask error\n");

	printf("%s", str);

	if (sigismember(&sigset, SIGINT)) printf("SIGINT ");
	if (sigismember(&sigset, SIGQUIT)) printf("SIGQUIT ");
	if (sigismember(&sigset, SIGUSR1)) printf("SIGUSR1 ");
	if (sigismember(&sigset, SIGALRM)) printf("SIGALRM ");

	/* remaining signals can go here */
	printf("\n");
	errno	= errno_save;
}
