#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>

int		quitflag;
sigset_t	mask;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t	wait = PTHREAD_COND_INITIALIZER;

int thr_fn(void *arg)
{
	int err, signo;

	for (;;) {
		err	= sigwait(&mask, &signo);
		if (err != 0) {
			printf("sigwait failed:%s\n", strerror(errno));
			exit(0);
		}

		switch (signo) {
			case SIGINT:
				printf("\ninterrupt\n");
				break;
			case SIGQUIT:
				pthread_mutex_lock(&lock);
				quitflag = 1;
				pthread_mutex_unlock(&lock);
				pthread_cond_signal(&wait);
				return (0);

			default:
				printf("unexpected signal %d\n", signo);
				exit(1);
		}
	}
}

int main(void)
{
	int		err;
	sigset_t	oldmask;
	pthread_t	tid;

	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGQUIT);

	if ((err = pthread_sigmask(SIG_BLOCK, &mask, &oldmask)) != 0) {
		printf("SIG_BLOCK error:%s\n", strerror(errno));
		exit(2);
	}

	err	= pthread_create(&tid, NULL, thr_fn, 0);
	if (err != 0) {
		printf("can't create thread:%s\n", strerror(errno));
		exit(3);
	}

	pthread_mutex_lock(&lock);
	while (quitflag == 0) {
		printf("main thread: I am in while\n");
		pthread_cond_wait(&wait, &lock);
	}
	pthread_mutex_unlock(&lock);

	/* SIGQUIT has been caught and is now blocked; do whatever*/
	quitflag = 0;

	/* reset signal mask which unblocks SIGQUIT */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
		printf("SIG_SETMASK error:%s\n", strerror(errno));
		exit(4);
	}

	return 0;
}
