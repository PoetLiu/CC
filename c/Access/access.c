#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#define ERROR_MAIN_PARAMETER	-1
#define ERROR_ACCESS_READ	-2
#define ERROR_OPEN_READ		-3

int main(int argc, char * argv[])
{
	if (argc != 2) {
		printf("usage: access <pathname>\n");
		return ERROR_MAIN_PARAMETER;
	}
	
	if (access(argv[1], R_OK) < 0 ) {
		printf("access error for %s\n", argv[1]);
		return ERROR_ACCESS_READ;
	}else
		printf("read access ok\n");

	if (open(argv[1], O_RDONLY) < 0) {
		printf("open file error for read\n");
		return ERROR_OPEN_READ;
	}else
		printf("open for reading ok\n");

	exit(0);
}
	
