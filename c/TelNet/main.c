#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <net/if.h>
#include <netinet/in.h>
#include <unistd.h>


#define SERVER_PORT 33333
#define DEBUG(fmt, args...)							\
	do{if (1)								\
		printf("DEBUG:%s:%d "fmt"\n", __FUNCTION__, __LINE__, ##args);	\
	}while(0)

#define ERROR(fmt, args...)							\
	do{if (1)								\
		printf("ERROR:%s:%d "fmt"(%d:%s)\n", __FUNCTION__, __LINE__, 	\
		##args, errno, strerror(errno));\
	}while(0)
static char *welcome	= "welcome to server !";

static int server_init()
{	
	int sock;
	struct sockaddr_in	serv_addr;

	sock	= socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (sock < 0) {
		ERROR("create socket faild");
		return -1;
	}

	bzero((void *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= htonl(INADDR_ANY);
	serv_addr.sin_port		= htons(SERVER_PORT);

	if (bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		ERROR("bind sock faild");
		close(sock);
		return -1;
	}

	if (listen(sock, 10) < 0) {
		ERROR("listen faild");
		return -1;
	}
	DEBUG("socket=%d\n", sock);
	return sock;
}

int process_rq(int fd)
{
	int wlen, len, pid, status;

	len	= strlen(welcome);
	if ((wlen = write(fd, welcome, len)) != len) {
		ERROR("write error");
		return -1;
	}

	pid	= fork();
	if ( pid < 0) {
		ERROR("fork faild");
		return -1;
	} else if(pid == 0) {
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDIN_FILENO);
		dup2(fd, STDERR_FILENO);
		setsid();
		system("last -1 $USER | head -n 1; cat /etc/motd");
		execl("/bin/bash", "--login", "-i", NULL);
		ERROR("execl faild");
		close(fd);
		exit(-1);
	} else {
		waitpid(pid, &status, 0);
		DEBUG("child pid:%d exited with status %d", pid, status);
	}


	return 0;
}

int main(void)
{
	int sk, fd, len;
	struct sockaddr_in cli_addr;

	len	= sizeof(cli_addr);
	sk	= server_init();

	if (sk < 0) {
		ERROR("server_init error");
		return -1;
	}

	while (1) {
		fd	= accept(sk, (struct sockaddr *)&cli_addr, &len);
		if (fd < 0) {
			ERROR("accept faild");
			continue;
		}
		DEBUG("get a call on (%s:%d)", inet_ntoa(cli_addr.sin_addr.s_addr), \
			ntohs(cli_addr.sin_port)); 
		
		process_rq(fd);
		close(fd);
	}


}
