/*
 *
 * Authored by Liu Peng on: Mon Nov  3 09:07:12 CST 2014
 *
 * @desc: shows how parent gets child status
 *
 * @func:
**/

#include <stdio.h>
#include <stdlib.h>

#define	DELAY	6

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
	int	child_status;
	int	high_8, low_7, bit_7;

	wait_rv	= wait(&child_status);
	printf("done waiting for %d. Wait returned:%d\n",	\
			childpid, wait_rv);

	high_8	= child_status >> 8;	/* pick high 8bits */
	low_7	= child_status & 0x7F;	/* pick low 7bits */
	bit_7	= child_status & 0x80;	/* pick 7th bit */

	printf("status:exit = %d, sig = %d, core = %d\n", high_8, low_7, bit_7);
}
