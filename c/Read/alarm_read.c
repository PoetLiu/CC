#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <setjmp.h>

#define MAXLINE	4096

static void	sig_alrm(int);
static jmp_buf	env_alrm;

int main(void)
{
	int	n;
	char	line[MAXLINE];

	if (signal(SIGALRM, sig_alrm) == SIG_ERR)
		printf("signal(SIGALRM error\n");

	if (setjmp(env_alrm) != 0) {
		printf("read timeout \n");
		return 1;
	}

	alarm(5);
	if ((n = read(STDIN_FILENO, line, MAXLINE)) < 0)
		printf("read error\n");
	alarm(0);

	write(STDOUT_FILENO, line, n);
	return 0;
}

static void sig_alrm(int signo)
{
	/* nothing to do, just return to interrupt the read*/
	longjmp(env_alrm, 1);
}
