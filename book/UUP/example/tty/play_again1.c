/*
   *
   * Authored by Liu Peng on: Thu Oct 30 10:33:36 CST 2014
   * play_again1.c
   * @desc purpose : ask if user wants another transaction
   *	   method  : ask a question, wait for yes/no answer
   *	   return  : 0=>yes, 1=>no
   *	   better  : do no echo inappropriate input
   * @func:
   */

#include <stdio.h>
#include <termios.h>

#define QUESTION "Do you want another transaction"

int get_response(char *);
void set_crmode();
int tty_mode(int how);

int main(void)
{
	int	response;

	tty_mode(0);				/* save tty mode */
	set_crmode();				/* set chr-by-chr mode */
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
			default:
				  printf("\nCannot understant %c,", input);
				  printf("Please type y or no \n");
		}
	}
}

/*
   * Funcion	: put file descriptor 0 (i.e stdin) into chr-by-chr mode
   *		  use bits in termios
   * Author	:
   *
   * arguments	: void
   * return	: void
   */
void set_crmode()
{
	struct	termios	ttystate;
	tcgetattr(0, &ttystate);
	ttystate.c_lflag	&= ~ICANON;
	ttystate.c_cc[VMIN]	= 1;
	tcsetattr(0, TCSANOW, &ttystate);
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
