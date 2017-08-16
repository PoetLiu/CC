#include <stdio.h>
#include <pwd.h>
#include <signal.h>
#include <errno.h>

static void
my_alarm(int sigo)
{
	struct passwd *rootptr;

	printf("in signal handler\n");
	if ((rootptr = getpwnam("root")) == NULL)
		printf("getpwnam(root) error:%s\n", strerror(errno));
	alarm(1);
	printf("out signal handle\n");
}

int 
main(void)
{
	struct passwd *ptr;

	signal(SIGALRM, my_alarm);
	alarm(1);
	for(;;) {
		printf("in for loop\n");
		if ((ptr = getpwnam("liupeng")) == NULL)
			printf("getpwnam error:%s\n", strerror(errno));
		if (strcmp(ptr->pw_name, "liupeng") != 0)
			printf("return value corrupted!, pw_name = %s\n", \
				ptr->pw_name);
	}
}
