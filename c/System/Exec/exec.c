#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

char *env_init[] = {"USER=unknown", "PATH=/tmp", NULL};

int main(void)
{
	pid_t	pid;
	
	if ((pid = fork()) < 0) {
		printf("fork error\n");
	} else if (pid == 0) {
		if (execle("/home/liupeng/program/Exec/hell", "hello", "arg1", (char *)0, env_init) < 0) {
			printf("execle error 1 :%s\n", strerror(errno));
			exit(1);
		}
		printf("Test 1: after execle\n");
	}

	printf("pid :%d\n", pid);
	if (waitpid(pid, NULL, 0) < 0)
		printf("waitpid error %d :%s\n", pid, strerror(errno));

	if ((pid = fork()) < 0) {
		printf("fork error 2 :%s\n", strerror(errno));
	}else if (pid == 0) {
		if (execlp("./hell", "he","arg1", (char *)0) < 0) {
			printf("execlp error 2 :%s\n", strerror(errno));
			exit(2);
		}
		printf("Test 2: after execlp\n");
	}

	exit(0);
}
