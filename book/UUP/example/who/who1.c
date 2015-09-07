/*
   *
   * Authored by Liu Peng on: Tue Oct 28 09:33:47 CST 2014
   *
   * @desc:
   *
   * @func : a first version of the who program
   *	     open, read UTMP file, and show results
 */	     	

#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>

#define SHOWHOST			/* include host on output */

void show_info(struct utmp* utbufp);

int main(void)
{
	struct utmp	current_record;	/* read info into here */
	int		utmpfd;		/* read from this fd */
	int		reclen = sizeof(current_record);

	if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) {
		perror(UTMP_FILE);	/* UTMP_FILE is in utmp.h */
		exit(1);
	}

	while (read(utmpfd, &current_record, reclen) == reclen)
		show_info(&current_record);

	close(utmpfd);
	return 0;
}

/*
   * Funcion	: display contents of the utmp struct in human readable 
   *		  form
   * Author	: UUP
   *
   * arguments	:
   * return	: void
   * note	: these sizes should not be hardwired
   */
void show_info(struct utmp* utbufp)
{
	printf("% -8.8s", utbufp->ut_user);	/* the user name */
	printf(" ");				/* a space */
	printf("% -8.8s", utbufp->ut_line);	/* the tty */
	printf(" ");		
	printf("% 10ld", utbufp->ut_tv.tv_sec);	/* logtime */
	printf(" ");
#ifdef SHOWHOST
	printf("(%s)", utbufp->ut_host);	/* the host */
#endif
	printf("\n");
}

