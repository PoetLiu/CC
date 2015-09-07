/*
 *
 * Authored by Liu Peng on: Fri Oct 31 10:04:55 CST 2014
 * --hello5.c--
 * @desc: bounce a message back and forth across the screen
 *	  complie cc hello5.c hello5 -lcurses
 * @func:
**/
#include <stdio.h>
#include <curses.h>

#define LEFTEDGE	10
#define RIGHTEDGE	30
#define ROW		10

int main(void)
{
	char	*message = "Hello";
	char	*blank	= "     ";
	int	dir	= +1;
	int	pos	= LEFTEDGE;

	initscr();				/* turn on curses */
						/* send requests */
	clear();				/* clear screen */
	while(1) {
		move(ROW, pos);			/* row10, col20 */
		addstr(message);
		move(LINES - 1, COLS - 1);	/* park the cursor */
		refresh();			/* show string */
		usleep(100000);
		move(ROW, pos);			/* erase string */
		addstr(blank);
		pos	+= dir;
		if (pos >= RIGHTEDGE)
			dir 	= -1;
		if (pos <= LEFTEDGE)
			dir	= +1;
	}
	endwin();				/* turn off curses */
}
