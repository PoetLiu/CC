/*
 *
 * Authored by Liu Peng on: Fri Nov  7 16:08:36 CST 2014
 *
 * @desc: controlled animation using two threads
 *	  one thread handles animation
 * 	  other thread handles keyboard input
 * @complie: cc tbounceld.c -o tbounceld -lpthread -lcurses
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <curses.h>
#include <unistd.h>

/* shared variables both threads use. These need a mutex */

#define	MESSAGE	"hello"
#define SPACE	"     "

int	row;				/* current row */
int	col;				/* current column */
int	dir;				/* where we are going */
int	delay;				/* delay between moves */

void *moving_msg();

int main(int ac, char *av[])
{
	int		ndelay;		/* new delay */
	int		c;		/* user input */
	pthread_t	msg_thread;	/* a thread */

	initscr();			/* init curses and tty */
	crmode();
	noecho();
	clear();

	row	= 10;			/* start here */
	col	= 0;
	dir	= 1;			/* add 1 to row number */
	delay	= 200;			/* 200ms = 0.2 seconds */

	if (pthread_create(&msg_thread, NULL, moving_msg, MESSAGE)) {
		fprintf(stderr, "error creating thread");
		endwin();
		exit(0);
	}

	while (1) {
		ndelay	= 0;
		c	= getch();

		if (c == 'Q') 
			break;
		if (c == ' ') 
			dir	= -dir;
		if (c == 'f' && delay > 2) 
			ndelay	= delay / 2;
		if (c == 's')
			ndelay	= delay * 2;
		if (ndelay > 0)
			delay	= ndelay;
	}

	pthread_cancel(msg_thread);
	endwin();
}

void *moving_msg(char *msg)
{
	while (1) {
		usleep(delay * 1000);	/* sleep a while */
		move(row, col - dir);	/* set cursor position */
		addstr(SPACE);
		move(row, col);		/* set cursor position */
		addstr(msg);		/* redo message */
		refresh();		/* and show it */

		/* move to next column and check for bouncing */
		col	+= dir;		/* move to new column */

		if (col <= 0 && dir == -1)
			dir	= 1;
		else if (col + strlen(msg) >= COLS && dir == 1)
			dir	= -1;
	}
}
