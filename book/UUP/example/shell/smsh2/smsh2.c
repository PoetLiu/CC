/*
 *
 * Authored by Liu Peng on: Mon Nov  3 11:30:46 CST 2014
 *
 * @desc: smsh2.c small - shell version 2
 *	  small shell that supports command line parsing
 *	  and if .. the .. else . fi logic(by calling process())
 * @func:
**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "smsh.h"

#define DEL_PROMPT	">"

int main(void)
{
	char	*cmdline, *prompt, **arglist;
	int	result;
	void	setup();

	prompt	= DEL_PROMPT;
	setup();

	while ((cmdline = next_cmd(prompt, stdin)) != NULL) {
		if (( arglist = splitline(cmdline)) != NULL) {
			result	= process(arglist);
			freelist(arglist);
		}
		free(cmdline);
	}

	return 0;
}

/*
 * Funcion	: initialize shell
 *		  
 * Author	:
 *
 * arguments	:
 * return	: nothing. calls fatal() if trouble
**/
void setup()
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n)
{
	fprintf(stderr, "Error :%s, %s\n", s1, s2);
	exit(n);
}
