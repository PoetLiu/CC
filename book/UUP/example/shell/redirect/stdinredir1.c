/*
 *
 * Authored by Liu Peng on: Mon Nov  3 16:26:00 CST 2014
 *
 * @desc: show how to redirect standard input by replacing file
 *	  descriptor 0 with a connection to a file.
 * @atcion: reads three lines from standard input, then
 *	    closes fd 0, opens a disk file, then reads in 
 *	    three more lines from standard input
**/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{

	int	fd;
	char	line[100];

	/* read and print three lines */
	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);

	/* redirect iput */
	close(0);
	fd	= open("/etc/passwd", O_RDONLY);
	if (fd != 0) {
		fprintf(stderr, "Could not open data as fd 0 \n");
		exit(1);
	}

	/* read and print three lines */
	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);

}
