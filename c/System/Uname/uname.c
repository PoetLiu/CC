#include <stdio.h>
#include <sys/utsname.h>
#include <errno.h>

int main(void)
{
	struct utsname un;

	if (uname(&un) == -1)
		printf("uname error:%s\n", strerror(errno));

	printf("sysname		: %s\n", un.sysname);
	printf("nodename	: %s\n", un.nodename);
	printf("release		: %s\n", un.release);
	printf("version		: %s\n", un.version);
	printf("machine		: %s\n", un.machine);

	return 0;
}
