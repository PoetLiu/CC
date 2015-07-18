#include <stdio.h>

struct user {
	char name[64];
	int id;
};

struct user users[1000000] = {0};

#define GET_ARRAY_SIZE(a)	(sizeof(a)/sizeof(a[0]))

int main(int argc, char **argv)
{
	int i = 0; 
	char buf[64];

	/*
	for (i = 0; i < GET_ARRAY_SIZE(users); i++) {
		snprintf(buf, sizeof(buf), "name %d", i);
		strcpy(users[i].name, buf);
	}
	*/

	if (!strcmp(argv[1], "-m")) {
		printf("muti times copy\n");
		for (i = 0; i < GET_ARRAY_SIZE(users) - 1; i++)
			memcpy(&users[i], &users[i+1], sizeof(struct user));
	} else  {
		printf("one time copy\n");
		memcpy(&users[0], &users[1], (GET_ARRAY_SIZE(users) - 1)* sizeof(struct user));
	}

//	for (i = 0; i < GET_ARRAY_SIZE(users); i++)
//		printf("%s\n", users[i].name);
	return 0;
}
