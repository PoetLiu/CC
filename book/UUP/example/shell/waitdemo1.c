/*
 *
 * Authored by Liu Peng on: Mon Nov  3 09:07:12 CST 2014
 *
 * @desc: shows how parent pauses until child finishes
 *
 * @func:
**/

#include <stdio.h>
#include <stdlib.h>

#define	DELAY	3

void	child_code();
void	parent_code();


int main(void)
{
	int	newpid;
	
	printf("before:mypid is %d\n", getpid());

	if ((newpid = fork()) == -1)
		perror("fork");
	else if (newpid == 0)
		child_code(DELAY);
	else
		parent_code(newpid);
}

/*
 * Funcion	: new process takes a nap and the exits
 *
 * Author	: 
 *
 * arguments	:
 * return	:
**/
void child_code(int delay)
{
	printf("child %d here. will sleep for %d seconds\n",	\
			getpid(), delay);
	sleep(delay);
	printf("child done. about to exit\n");
	exit(17);
}

/*
 * Funcion	: parent waits for child then prints a message
 *
 * Author	:
 *
 * arguments	:
 * return	:
**/
void parent_code(int childpid)
{
	int	wait_rv;	/* return value from wait() */
	wait_rv	= wait(NULL);
	printf("done waiting for %d. Wait returned:%d\n",	\
			childpid, wait_rv);
}
