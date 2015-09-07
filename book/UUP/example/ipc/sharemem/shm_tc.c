/*
 *
 * Authored by Liu Peng on: Mon Nov 10 13:39:33 CST 2014
 *
 * @desc: the time client using shared memory, a bizzare application
 *
 * @func:
**/
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <time.h>

#define	TIME_MEM_KEY	99		/* kind of like a port number */
#define	SEG_SIZE	((size_t)100)	/* size of segmen */
#define	oops(m, x)	{perror(m);exit(x);}

int main(void)
{
	int	seg_id;
	char	*mem_ptr;
	long	now;

	/* create a shared memory segment */
	seg_id	= shmget(TIME_MEM_KEY, SEG_SIZE, 0777);
	if (seg_id == -1)
		oops("shmget", 1);

	/* attach to it and get a pointer fo where it attaches */
	mem_ptr	= shmat(seg_id, NULL, 0);
	if (mem_ptr == (void *)-1)
		oops("shmat", 2);
	printf("The time, direct from memory:..%s", mem_ptr);

	shmdt(mem_ptr);			/* detach, but not needed here */
}

