/*
   *
   * Authored by Liu Peng on: Thu Oct 30 10:33:36 CST 2014
   *
   * @desc purpose : ask if user wants another transaction
   *	   method  : ask a question, wait for yes/no answer
   *	   return  : 0=>yes, 1=>no
   *	   betrer  : eliminate need to press retrun
   * @func:
   */

#include <stdio.h>
#include <termios.h>

#define QUESTION "Do you want another transaction"

int get_response(char *);

int main(void)
{
	int	response;

	response = get_response(QUESTION);
	
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
	printf("%s(y/n)?", question);

	while (1) {
		switch (getchar()) {
			case 'y':
			case 'Y': return 0;
			case 'n':
			case 'N':
			case EOF: return 1;
		}
	}
}
