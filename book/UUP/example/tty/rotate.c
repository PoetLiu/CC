/*
   *
   * Authored by Liu Peng on: Thu Oct 30 10:14:32 CST 2014
   *
   * @desc rotate.c : map a->b, b->c, .. z->a
   *	   
   * @func:useful for showing tty modes
   */

#include <stdio.h>
#include <ctype.h>

int main(void)
{
	int c;
	 
	while ((c = getchar()) != EOF) {
		if (c == 'z')
			c	= 'a';
		else if (islower(c))
			c++;
		
		putchar(c);
	}
	return 0;
}

