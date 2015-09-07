/*
 *
 * Authored by Liu Peng on: Mon Nov  3 09:34:04 CST 2014
 *
 * @desc: Solves the 'one-shot' problem of version 1
 *	  Uses execvp(), but fork()s first so that the
 *	  shell waits around to perform another command
 *
 * @New problem: shell catches signals. Run vi, press ^C.
**/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define	MAXARGS	20		/* cmdline args */
#define ARGLEN	100		/* token length */

char	*makestring();
void	execute(char *arglist[]);
char	*makestring(char *buf);
void	free_list(char *arglist[]);

int main(void)
{
	char	*arglist[MAXARGS + 1];	/* an array of ptrs */
	int	numargs;		/* index into array */
	char	argbuf[ARGLEN];		/* read stuff here */

	numargs	= 0;
	while (numargs < MAXARGS) {
		printf("Arg[%d]?", numargs);
		if (fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n')
			arglist[numargs++]	= makestring(argbuf);
		else {
			if (numargs > 0) { /* any args ? */
				arglist[numargs]	= NULL;
				execute(arglist);	/* do it */
				free_list(arglist);
				numargs	= 0;
			}
		}
	}
	return 0;
}

void free_list(char *arglist[])
{
	int	i;
	for (i = 0; arglist[i] != NULL; i++) {
		free(arglist[i]);
		arglist[i]	= NULL;
	}
	
}

/*
 * Funcion	: use fork and execvp and wait to do it
 *
 * Author	:
 *
 * arguments	:
 * return	:
**/
void execute(char *arglist[])
{
	int	pid, exitstatus;	/* for child */

	pid	= fork();
	switch (pid) {
		case -1:
			perror("fork failed");
			exit(1);
		case 0:
			execvp(arglist[0], arglist);	/* do it */
			perror("execvp failed");
			exit(1);
		default:
			while (wait(&exitstatus) != pid);
			printf("child exited with status %d, %d\n", \
				exitstatus >> 8, exitstatus & 0x7F);
	}
}

/*
 * Funcion	: trim off newline and create storage for the string
 *
 * Author	:
 *
 * arguments	:
 * return	:
**/
char *makestring(char *buf)
{
	char	*cp;

	buf[strlen(buf) - 1]	= '\0';		/* trim newline */
	cp	= malloc(strlen(buf) + 1);	/* get memory */
	if (cp == NULL) {
		fprintf(stderr, "no memory\n");
		exit(1);
	}

	strcpy(cp, buf);			/* copy chars */
	return cp;
}
