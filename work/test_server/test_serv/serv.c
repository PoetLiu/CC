#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include "mylib.h"

#define PORT 8888
int sk;

static pid_t exe_cmd(char *cmd)
{
	pid_t pid = 0;

	DEBUG("cmd len:%d %s\n", (int)strlen(cmd), cmd);
	pid = fork();
	if (pid < 0) {
		DEBUG("fork() error\n");
		return -1;
	} else if (pid == 0) {
		close_parent_fd();
		system(cmd);	
		exit(0);
	}
	
	waitpid(pid, NULL, 0);
	return pid;
}

static int str_eat_end(char *str, int len)
{
	int i;

	for (i = 0; i < len; i++) {
//		DEBUG("%d\n", str[i]);
		if (str[i] == '\r' || str[i] == '\n')
			str[i] = '\0';
	}
	return i;
}

static void process_req(int fd)
{
	char buf[2048];
	char cmd[1024];
	int rlen;

	memset(cmd, 0, sizeof(cmd));
	memset(buf, 0, sizeof(buf));

	if ((rlen = read(fd, buf, 2048)) < 0)
		DEBUG("read error\n");

	// remove \n
	buf[rlen]	= '\0';
	str_eat_end(buf, rlen);
	//DEBUG("get msg len:%d %s\n", strlen(buf), buf);

	close(fd);

	if (!strncmp(buf, "get", 3)) {
		//DEBUG("run get.sh, getting file\n");
		if (strlen(buf) > 3)
			sprintf(cmd, "./get.sh%s", &buf[3]); 
		else
			sprintf(cmd, "./get.sh");
		
		if (!exe_cmd(cmd))
			DEBUG("run %s error\n", cmd);
	} else if (!strncmp(buf, "run", 3)) {
		sprintf(cmd, "./run.sh%s", &buf[3]); 
		if (!exe_cmd(cmd))
			DEBUG("run ./run.sh error\n");
	} else 
		DEBUG("unknown cmd\n");
	
}
static void server_accept(int sk)
{
	struct sockaddr_in addr;
	int fd, len = sizeof(&addr);

	fd	= accept(sk, (struct sockaddr *)&addr, (socklen_t *)&len);
	if (fd < 0) {
		DEBUG("accept faild\n");
		return;
	}

	//DEBUG("get a call from %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	process_req(fd);
}

static void sig_term(int signo)
{
	printf("catch a SIGTERM\n");
	close(sk);
	exit(0);
}

int main(int argc, char **argv)
{
	int ret = -1;

	sk	= socket_init(0, 1);
	if (sk < 0)
		return -4;

	if (signal(SIGTERM, sig_term) == SIG_ERR)
		return -5;

	DEBUG("init ok\n");

	while (1) {
		int max_fd;
		fd_set read_fds;
		struct timeval tv;
		
		max_fd		= 0;
		tv.tv_sec	= 1;
		tv.tv_usec	= 0;

		FD_ZERO(&read_fds);
		FD_SET(sk, &read_fds);

		if (max_fd < sk)
			max_fd	= sk;
	
		ret	= select(max_fd + 1, &read_fds, NULL, NULL, &tv);
		if (ret == -1) {
			if (errno == EINTR || errno == EAGAIN)
				continue;
		} else {
			if (FD_ISSET(sk, &read_fds))
				server_accept(sk);
		}

	}	
	return 0;
}
