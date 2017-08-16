#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

	if (argc != 2){
		printf("usage: unlink <pathname>\n");
		return -1;
	}
	
	if (unlink(argv[1]) != 0) {
		printf("unlink error: %s\n", strerror(errno));
		return -2;
	}else {
		printf("%s :unlink ok\n", argv[1]);
	}
	return 0;
}
