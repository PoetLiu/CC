#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void)
{
	pid_t	pid;

	if ((pid = fork()) < 0) {
		printf("fork error\n");
	} else if (pid == 0) {
		sleep(2);
		if ((pid = fork()) < 0)
			printf("fork error\n");
		else if (pid > 0)
			exit(0); /* parent from second fork == first child*/

		/*
		 * We're the second child; our parent becomes init as soon
		 * as our real parent calls exit() in the statement above.
		 * Here's where we'd continue executing, knowing that when
		 * we're done, init will reap our status.
		 */
		sleep(2);
		printf("second child, parent pid = %d\n", getppid());
		exit(0);
	}

	printf("waiting for process %d exit!\n", pid);
	if (waitpid(pid, NULL, 0) != pid)
		printf("waitpid error\n");

	/*
	 * We're the parent (the original process); we continue executing, 
	 * Knowing that we're not the parent of the second child.
	 */
	exit(0);
}
