#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int ac, char *av[])
{
	struct stat infobuf;

	if (ac != 2) {
		printf("usage:%s file\n", av[0]);
		exit(1);
	}

	if (stat(av[1], &infobuf) == -1) {
		perror(av[1]);
	} else {
		printf("The size of %s is %d\n", av[1], infobuf.st_size);
	}

	return 0;
}
