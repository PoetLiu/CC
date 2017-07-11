#include <stdio.h>

void msg_handler(char *msg)
{
	char c = *msg;
	return;
}

int main(void)
{
	msg_handler(NULL);
	return 0;
}
