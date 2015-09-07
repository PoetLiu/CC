/*
 *
 * Authored by Liu Peng on: Fri Oct 31 10:04:55 CST 2014
 *
 * @desc: show the minimal calls needed to use curses
 *	  initialize, draw stuff, wait for input, quit
 * @func:
**/
#include <stdio.h>
#include <curses.h>

int main(void)
{
	initscr();			/* turn on curses */
					/* send requests */
	clear();			/* clear screen */
	move(10, 20);			/* row10, col20 */
	addstr("Hello, world");		/* add s string */
	move(LINES-1, 0);		/* move to LL */
	refresh();			/* update the screen */
	getch();			/* wait for user input */
	endwin();			/* turn off curses */
}
