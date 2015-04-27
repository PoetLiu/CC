#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>

void *routine(){
	int i=898;
	fputs("This is a pthread acting func\n", stdout);
//	execlp("ls","ls","-al",NULL);//zhu yi xian cheng kongjian you xian
	i++;
	fprintf(stdout, "i=%d\n", i);
}


int main(int argc,char *argv[])
{
	pid_t pid;
	pid = fork();
	pthread_t a_thread;

	if (pid < 0) {
		perror("Creating child process failed!!!");
		exit(-1);    
	} else if (pid==0) {
		if (pthread_create(&a_thread, NULL, routine, NULL)<0) {
			perror("creating pthread failed!!!");
			exit(-1);
		}
		printf("kan kan wo hai neng da yin shu chu bu ne hahhah\n");
	}
	
	printf("back to parent\n");
	exit(0);
}




























