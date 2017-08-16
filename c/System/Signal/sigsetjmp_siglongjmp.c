#include <stdio.h>
#include <setjmp.h>
#include <time.h>
#include <signal.h>
#include <errno.h>

static void sig_usr1(int), sig_alrm(int);
static void pr_mask(const char *str);

static sigjmp_buf		jmpbuf;
static volatile sig_atomic_t	canjump;

int main(void)
{
	if (signal(SIGUSR1, sig_usr1) == SIG_ERR)
		printf("signal(SIGUSR1) error");

	if (signal(SIGALRM, sig_alrm) == SIG_ERR)
		printf("signal(SIGALRM) error");

	pr_mask("starting main: ");

	if (sigsetjmp(jmpbuf, 1)) {
		pr_mask("ending main:");
		return 1;
	}

	canjump	= 1;
	
	for(;;)
		pause();

	return 0;
}

static void sig_usr1(int signo)
{
	time_t	starttime;

	if (canjump == 0)
		return;

	pr_mask("starting sig_usr1: ");
	alarm(3);
	starttime	= time(NULL);
	for(;;)
		if (time(NULL) > starttime + 5)
			break;
	pr_mask("finishing sig_usr1: ");

	canjump	= 0;
	siglongjmp(jmpbuf, 1);
}

static void sig_alrm(int signo)
{
	pr_mask("in sig_alrm: ");
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
