/*
   *
   * Authored by Liu Peng on: Thu Oct 30 10:33:36 CST 2014
   * play_again2.c
   * @desc purpose : ask if user wants another transaction
   *	   method  : set tty into char-by-char mode and no-echo mode
   *		     and no-delay mode
   *	   return  : 0=>yes, 1=>no, 2=>timeout
   *	   better  : reset terminal mode on Interrupt
   * @func:
   */

#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>

#define ASK		"Do you want another transaction"
#define TRIES		5
#define SLEEPTIME	1
#define	BEEP		putchar('\a')

int	get_response(char *question, int maxtries);
void	set_cr_noecho_mode();
int	tty_mode(int how);
int	get_ok_char();
void	set_nodelay_mode();

int main(void)
{
	int	response;

	tty_mode(0);				/* save tty mode */
	set_cr_noecho_mode();			/* set -icanon, -echo */
	set_nodelay_mode();			/* noinput => EOF */
	response = get_response(ASK, TRIES);	/* get some answer */
	tty_mode(1);				/* restore tty orig mode */
	return response;
}

/*
   * Funcion	: ask a question and wait for a y/n answer or maxtries
   *		  use getchar and ignore non y/n answers
   * Author	:
   *
   * arguments	:
   * return	: 0=>yes, 1=>no, 2=>timeout
   */
int get_response(char *question, int maxtries)
{
	int	input;
	printf("%s(y/n)?", question);			/* ask */
	fflush(stdout);					/* force output */

	while (1) {
		sleep(SLEEPTIME);			/* wait a bit */
		input	= tolower(get_ok_char());	/* get next chr */
		if (input == 'y')
			return 0;
		if (input == 'n')
			return 1;
		if (maxtries-- == 0)
			return 2;
		BEEP;
	}
}

/*
 * Funcion	: skip over non - legal chars and return y, Y, n, N or EOF
 *
 * Author	:
 *
 * arguments	:
 * return	:
**/
int get_ok_char()
{
	int c;
	while ((c = getchar()) != EOF && strchr("yYnN", c) == NULL)
		;
	return c;
}


/*
   * Funcion	: put file descriptor 0 (i.e stdin) into chr-by-chr
   *		  and noecho mdoe
   *		  use bits in termios
   * Author	:
   *
   * arguments	: void
   * return	: void
   */
void set_cr_noecho_mode()
{
	struct	termios	ttystate;
	tcgetattr(0, &ttystate);		/* read curr. setting */
	ttystate.c_lflag	&= ~ICANON;	/* no buffering */
	ttystate.c_lflag	&= ~ECHO;	/* no echo either */
	ttystate.c_cc[VMIN]	= 1;		/* get 1 char at a time */
	tcsetattr(0, TCSANOW, &ttystate);	/* install settings */
}

/*
 * Funcion	: put file descriptor 0 into no-delay mode
 *		  use fcntl to set bits
 * notes	: tcsetattr() will do something similar, but it is complicated
**/
void set_nodelay_mode()
{
	int	termflags;

	termflags	= fcntl(0, F_GETFL);	/* read curr.settings */
	termflags	|= O_NDELAY;		/* flip on nodelay bit */
	fcntl(0, F_SETFL, termflags);		/* and install 'em */
	
}

/*
   * Funcion	: handles termios and fcntl falgs
   *
   * Author	:
   *
   * arguments	: how 0-> save current mode
   *		      1-> restore mode
   * return	: void
   */
int tty_mode(int how)
{
	static struct termios	original_mode;
	static int		original_flags;

	if (!how) {
		tcgetattr(0, &original_mode);
		original_flags	= fcntl(0, F_GETFL);
	} else {
		tcsetattr(0, TCSANOW, &original_mode);
		fcntl(0, F_SETFL, original_flags);
	}
}

