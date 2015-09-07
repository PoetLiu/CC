/*
 *
 * Authored by Liu Peng on: Mon Nov 10 14:20:50 CST 2014
 *
 * @desc: read the current date/time from a file
 * @usaeg: file_tc filename
 * @func: fcntl() based locking
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <fcntl.h>

#define	oops(m, x)	{perror(m);exit(x);}
#define	BUFLEN		10

void lock_operation(int fd, int op);
int main(int ac, char *av[])
{
	int	fd;
	int	nread;
	char	buf[BUFLEN];

	if (ac != 2) {
		fprintf(stderr, "usage: file_tc filename\n");
		exit(1);
	}

	if ((fd = open(av[1], O_RDONLY)) == -1)
		oops(av[1], 3);

	lock_operation(fd, F_RDLCK);

	while ((nread = read(fd, buf, BUFLEN)) > 0)
		write(1, buf, nread);

	close(fd);
}

void lock_operation(int fd, int op)
{
	struct flock	lock;

	lock.l_whence	= SEEK_SET;
	lock.l_start	= lock.l_len	= 0;
	lock.l_pid	= getpid();
	lock.l_type	= op;

	if (fcntl(fd, F_SETLKW, &lock) == -1)
		oops("lock operation", 6);
}
