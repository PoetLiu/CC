/*
   *
   * Authored by Liu Peng on: Tue Oct 28 09:33:47 CST 2014
   *
   * @desc: who3.c - who with buffered reads
   *		   - surpresses empty records
   *		   - formats time nicely
   *		   - buffers input(using utmplib.c)
   *
   * @func : a 3td version of the who program
   *	     open, read UTMP file, and show results
 */	     	

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <utmp.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#define SHOWHOST			/* include host on output */

void show_info(struct utmp* utbufp);
void show_time(const time_t timeval);
extern utmp_open(char *);
extern struct utmp * utmp_next();
extern utmp_close();

int main(void)
{
	struct utmp * utbufp;		/* read info into here */

	if (utmp_open(UTMP_FILE) == -1) {
		perror(UTMP_FILE);	/* UTMP_FILE is in utmp.h */
		exit(1);
	}

	while ((utbufp = utmp_next()) != ((struct utmp *)NULL))
		show_info(utbufp);

	utmp_close();
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
	if (utbufp->ut_type != USER_PROCESS)
		return;
	printf("% -8.8s", utbufp->ut_user);	/* the user name */
	printf(" ");				/* a space */
	printf("% -8.8s", utbufp->ut_line);	/* the tty */
	printf(" ");		
	show_time(utbufp->ut_tv.tv_sec);
	printf(" ");
#ifdef SHOWHOST
	printf("(%s)", utbufp->ut_host);	/* the host */
#endif
	printf("\n");
}

void show_time(const time_t timeval)
{
	 char *cp;
	 cp = ctime(&timeval);
	 printf("%12.12s", cp + 4);
}
