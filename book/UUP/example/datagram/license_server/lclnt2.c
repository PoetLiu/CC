/*
 *
 * Authored by Liu Peng on: Wed Nov  5 16:21:42 CST 2014
 *
 * @desc: License server client version 1
 *	  link with lclnt_funcs1.o dgram.o
 * @func:
**/

#include <stdio.h>
#include <stdlib.h>
/*
 * Funcion	: the main work of the application goes here
 *
 * Author	:
 *
 * arguments	:
 * return	:
**/
void do_regular_work()
{
	printf("SuperSleep verson 1.0 Running-Licensed Software\n");
	sleep(30);	/* our patented sleep algorithm */
	if (do_validate() != 0)
		exit(0);
	sleep(20);
}

int main(int ac, char *av[])
{
	setup();
	if (get_ticket() != 0)
		exit(0);
	do_regular_work();
	release_ticket();
	shut_down();
}
