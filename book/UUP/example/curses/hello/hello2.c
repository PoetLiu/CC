/*
 *
 * Authored by Liu Peng on: Fri Oct 31 10:04:55 CST 2014
 * --hello2.c--
 * @desc: show the minimal calls needed to use curses
 *	  initialize, draw stuff, wait for input, quit
 * @func:
**/
#include <stdio.h>
#include <curses.h>

int main(void)
{
	int i;
	initscr();				/* turn on curses */
						/* send requests */
	clear();				/* clear screen */
	for (i = 0; i < LINES; i++) {
		move(i, i + 1);			/* row10, col20 */
		if (i % 2 == 1)
			standout();
		addstr("Hello, world");		/* add s string */
		if (i % 2 == 1)
			standend();
	}
	refresh();				/* update the screen */
	getch();				/* wait for user input */
	endwin();				/* turn off curses */
}
