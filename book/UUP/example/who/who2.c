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
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#define SHOWHOST			/* include host on output */

void show_info(struct utmp* utbufp);
void show_time(const time_t timeval);

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
	if (utbufp->ut_type != USER_PROCESS)
		return;
	printf("% -8.8s", utbufp->ut_user);	/* the user name */
	printf(" ");				/* a space */
	printf("% -12.12s", utbufp->ut_line);	/* the tty */
	printf(" ");		
	show_time(utbufp->ut_tv.tv_sec);
	printf(" ");
#ifdef SHOWHOST
	if (utbufp->ut_host[0] != '\0')
		printf("(%s)", utbufp->ut_host);/* the host */
#endif
	printf("\n");
}

void show_time(const time_t timeval)
{
	 char *cp;
	 struct tm result;
#if 0
	 cp = ctime(&timeval);
	 printf("%12.12s", cp + 4);
#endif
	 if (localtime_r(&timeval, &result) == NULL)
		 perror("gmtime_r");

	 printf("%4.4d-%2.2d-%2.2d %2.2d:%2.2d", 			\
		result.tm_year + 1900, result.tm_mon, result.tm_mday,	\
		result.tm_hour, result.tm_min);
}
