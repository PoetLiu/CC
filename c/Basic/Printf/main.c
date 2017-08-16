/* By default,  the
 * arguments are used in the order given, One can also specify 
 * explicitly which  argument is taken, at each place where an 
 * argument is required, by writing "%m$" instead of '%' and 
 * "*m$" instead of '*', where the decimal integer m denotes 
 * the position in the argument list of the desired argument, 
 * indexed starting from 1.  Thus,
 *          printf("%*d", width, num);
 *      and
 *          printf("%2$*1$d", width, num);
 *      are  equivalent.
 * more details use cmd "man 3 printf", see "Format..."
*/

#include <stdio.h>

int main(int ac, char *av[])
{
	printf("%*d", atoi(av[1]), atoi(av[2]));
	printf("%2$*d", atoi(av[1]), atoi(av[2]));
}
