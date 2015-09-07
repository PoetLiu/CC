/*
 *
 * Authored by Liu Peng on: Fri Nov  7 16:36:01 CST 2014
 *
 * @desc : animate several strings using threads, curses
 *	   usleep()
 * 	   bigidea one thread for each animated string
 *	   one thread for keyboard control
 *	   shared variables for communication
 * @complie : cc tanimate.c -lcurses -lpthread -o tanimate
 * @todo needs locks for shared variables
 * 	 nice to put screen handling in its own thread
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <pthread.h>

#define	MAXMSG	10			/* limit to number of strings */
#define	TUNIT	20000			/* timeunits in microseconds */

void *animate();

struct propset {
	char 	*str;			/* the message */
	int	row;			/* the row */
	int	delay;			/* delay in time units */
	int	dir;			/* +1 or -1 */
	pthread_mutex_t	lock;
};

pthread_mutex_t	mx	= PTHREAD_MUTEX_INITIALIZER;

int main(int ac, char *av[])
{
	int		c;		/* user input */
	pthread_t	thrds[MAXMSG];	/* the threads */
	struct propset	props[MAXMSG];	/* properties of string */
	int		num_msg;	/* number of strings */
	int		i;

	if (ac == 1) {
		printf("usge: tanimate string...\n");
		exit(1);
	}

	num_msg	= setup(ac - 1, av + 1, props);

	/* create all the threads */
	for ( i = 0; i < num_msg; i++) {
		if (pthread_create(&thrds[i], NULL, animate, &props[i])) {
			fprintf(stderr, "error creating thread");
			endwin();
			exit(0);
		}
	}

	/* process user input */
	while (1) {
		c	= getch();
		if (c == 'Q') break;
		if (c == ' ') 
			for (i = 0; i < num_msg; i++) {
				pthread_mutex_lock(&props[i].lock);	/* only one thread */
				props[i].dir	= -props[i].dir;
				pthread_mutex_unlock(&props[i].lock);	/* donewith curses */
			}
		if (c >= '0' && c <= '9') {
			i	= c - '0';
			if (i < num_msg) {
				pthread_mutex_lock(&props[i].lock);	/* only one thread */
				props[i].dir	= -props[i].dir;
				pthread_mutex_unlock(&props[i].lock);	/* donewith curses */
			}
		}
	}

	/* cancle all the threads */
	pthread_mutex_lock(&mx);
	for (i = 0; i < num_msg; i++)
		pthread_cancel(thrds[i]);
	endwin();
	return 0;
}

int setup(int nstrings, char *strings[], struct propset props[])
{
	int	num_msg	= (nstrings > MAXMSG ? MAXMSG : nstrings);
	int	i;

	/* assign rows and velocities to each string */
	srand(getpid());
	for (i = 0; i < num_msg; i++) {
		props[i].str	= strings[i];		/* the message */
		props[i].row	= i;			/* the row */
		props[i].delay	= 1 + (rand() % 15);	/* a speed */
		props[i].dir	= ((rand() % 2) ? 1 : -1); /* +1 or -1 */
		pthread_mutex_init(&props[i].lock, NULL);
	}

	/* set up curses */
	initscr();
	crmode();
	noecho();
	clear();
	mvprintw(LINES - 1, 0, "'Q' to quit, '0'..'%d' to bounce",	\
		num_msg - 1);
	return num_msg;
}

/* the code that runs in each thread */
void *animate(void *arg)
{
	struct propset *info	= arg;		/* point to info block */
	int	len	= strlen(info->str) + 2; /* +2 for padding */
	int	col	= rand() % (COLS - len - 3); /* space for padding */

	while (1) {
		usleep(info->delay * TUNIT);

		pthread_mutex_lock(&mx);	/* only one thread */
		move(info->row, col);		/* can call curses */
		addch(' ');			/* at the same time */
		addstr(info->str);		/* Since I doubt it is */
		addch(' ');			/* reentrant */
		move(LINES - 1, COLS - 1);	/* pack cursor */
		refresh();			/* and show it */
		pthread_mutex_unlock(&mx);	/* donewith curses */
			
		/* move item to next column and check for bouncing */
		col	+= info->dir;

		if (col <= 0 && info->dir == -1)
			info->dir	= 1;
		else if (col + len >= COLS && info->dir == 1)
			info->dir	= -1;
	}
}
