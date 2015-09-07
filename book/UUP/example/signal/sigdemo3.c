/*
 *
 * Authored by Liu Peng on: Fri Oct 31 15:40:48 CST 2014
 *
 * @desc: show answers to signal questions
 * question1 does the handler stay in effect after a signal arrives?
 * question2 what if a signalX arrives while handling signalX?
 * question3 what if a signalX arrives while handling signalY?
 * question4 what happens to read() when a signal arrives?
 * @func:
**/

#include <stdio.h>
#include <signal.h>

#define INPUTLEN 100

void inthandler(int);
void quithandler(int);

int main(int ac, char *av[])
{
	char	input[INPUTLEN];
	int	nchars;

	signal(SIGINT, inthandler);
	signal(SIGQUIT, quithandler);

	do {
		printf("\nType a message\n");
		nchars	= read(0, input, (INPUTLEN - 1));
		if (nchars == -1)
			perror("read returned an error");
		else {
			input[nchars] = '\0';
			printf("You typed:%s", input);
			fflush(0);
		}
	} while (strncmp(input, "quit", 4) != 0);
	return 0;
}

void inthandler(int s)
{
	printf("Received signal %d..waiting\n", s);
	sleep(2);
	printf("Leaving inthandler\n");
}

void quithandler(int s)
{
	printf("Received signal %d..waiting\n", s);
	sleep(3);
	printf("Leaving quithandler\n");
}
