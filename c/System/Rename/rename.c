#include <stdio.h>
#include <errno.h>

int main(int argc, char *argv[1])
{
	if (argc != 3) {
		printf("usage: %s <oldname> <newname>\n", argv[0]);
		return -1;
	}

	if (rename(argv[1], argv[2]) != 0) {
		printf("rename error: %s\n", strerror(errno));
		return -2;
	}else {
		printf("rename ok\n");
	}

	return 0;
}
