/*
   *
   * Authored by Liu Peng on: Thu Oct 30 10:33:36 CST 2014
   * play_again2.c
   * @desc purpose : ask if user wants another transaction
   *	   method  : set tty into char-by-char mode and no-echo mode
   *	   return  : 0=>yes, 1=>no
   *	   better  : timeout if users walks away
   * @func:
   */

#include <stdio.h>
#include <termios.h>

#define QUESTION "Do you want another transaction"

int get_response(char *);
void set_cr_noecho_mode();
int tty_mode(int how);

int main(void)
{
	int	response;

	tty_mode(0);				/* save tty mode */
	set_cr_noecho_mode();			/* set -icanon, -echo */
	response = get_response(QUESTION);	/* get some answer */
	tty_mode(1);				/* restore tty mode */
	return response;
}

/*
   * Funcion	: ask a question and wait for a y/n answer
   *		  use getchar and ignore non y/n answers
   * Author	:
   *
   * arguments	:
   * return	: 0=>yes, 1=>no
   */
int get_response(char *question)
{
	int input;
	printf("%s(y/n)?", question);

	while (1) {
		switch (input = getchar()) {
			case 'y':
			case 'Y': return 0;
			case 'n':
			case 'N':
			case EOF: return 1;
		}
	}
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
   * Funcion	: save/restore mode
   *
   * Author	:
   *
   * arguments	: how 0-> save current mode
   *		      1-> restore mode
   * return	: void
   */
int tty_mode(int how)
{
	static struct termios original_mode;

	if (!how)
		tcgetattr(0, &original_mode);
	else
		return tcsetattr(0, TCSANOW, &original_mode);
}
