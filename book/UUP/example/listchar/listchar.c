/*
   *
   * Authored by Liu Peng on: Wed Oct 29 16:03:23 CST 2014
   *
   * @desc purpose : list individually all the chars are seen on input
   *	   ouput   : char and ascii code, one pair per line
   *	   input   : stdin, until the letter Q
   *	   notes   : usesful to show that buffering/editing exists
   * @func:
   */
#include <stdio.h>

int main(void)
{
	int	c, n = 0;

	while ((c = getchar()) != 'Q')
		printf("char %3d if %c code %d\n", n++, c, c);
	return 0;
}
