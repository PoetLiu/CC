#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include "nc_ipc.h"

#define DEBUG(fmt,args...) do{console_printf("%s=>%s=>%d   "fmt,\
 	     __FILE__, __FUNCTION__, __LINE__, ##args);}while(0)
#define CHECK_P_VALID(p,ret) do{if (!p){DEBUG(#p" is invalid\n");return ret;}}while(0)

#define PORT 8888
int sk;

static int socket_init()
{
        int sk;
        struct sockaddr_in addr;
        int reuse = 1;

        sk = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sk < 0) {
                DEBUG("create socket failed\n");
                return -1;
        }

        if (setsockopt(sk, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) == -1) {
                DEBUG("set SO_REUSEADDR error\n");
                return -1;
        }

        //bind
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(PORT);
        if (bind(sk, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                DEBUG("bind failed\n");
                close(sk);
                return -1;
        }

        //listen
        if (listen(sk, 10) < 0) {
                DEBUG("listen failed\n");
                close(sk);
                return -1;
        }

        DEBUG("socket=%d\n", sk);
        return sk;
}

int close_parent_fd(void)
{
        struct dirent **namelist = NULL;
        int n = 0;
        char dir[2048] = {0};

        sprintf(dir,"/proc/%d/fd", getpid());

        n = scandir(dir, &namelist, 0, alphasort);
        if(n < 0){
                return -1;
        }

        while(n--){
                int fd =  0;
                if(strcmp(".", namelist[n]->d_name) == 0){
                        free(namelist[n]);
                        continue;
                }
                if(strcmp("..", namelist[n]->d_name) == 0){
                        free(namelist[n]);
                        continue;
                }
                fd = atoi(namelist[n]->d_name);
                if(fd == 0 || fd == 1 || fd == 2){
                        free(namelist[n]);
                        continue;
                }
                close(fd);
                free(namelist[n]);
        }

        free(namelist);

        return 0;
}


static pid_t exe_cmd(char *cmd)
{
	pid_t pid = 0;

	DEBUG("cmd len:%d %s\n", strlen(cmd), cmd);
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

static pid_t pid_get(char *name)
{
	FILE *fp;
	char *ret = NULL;
	char buf[1024] = {0};
	char cmdline[1024] = {0};
	pid_t	pid = -1;

	CHECK_P_VALID(name, -1);
	snprintf(cmdline, sizeof(cmdline), "pidof %s", name);
	DEBUG("cmdline:%s\n", cmdline);

	fp	= popen(cmdline, "r");
	if (!fp) {
		DEBUG("popen faild\n");	
		return -1;
	}
	
	ret 	= fgets(buf, sizeof(buf), fp);
	if (!ret) {
		DEBUG("get pipe data faild\n");	
		return -1;
	}

	sscanf(buf, "%d", &pid);
	DEBUG("buf:%s pid:%d\n", buf, pid);
	pclose(fp);

	return pid;
}

static int app_stop(pid_t pid) 
{
	msg_app_t msg;

	msg.comm.key	= MSG_STOP;	
	msg.comm.gid	= NLKMSG_GRP_APP;
     	nlk_msg_send(0, pid, 0, &msg, sizeof(msg_app_t));

	sleep(5);

	return 0;
}

static int proc_stop(pid_t pid)
{
	char cmd[1024] = {0};

	snprintf(cmd, sizeof(cmd), "kill -9 %d", pid);
	system(cmd);

	return 0;
}

static int proc_name_check(char *name)
{
	if (strstr(name, "test_serv"))
		return 1;
	else 
		return 0;

}

static int process_stop(char *name)
{
	pid_t	pid;
	CHECK_P_VALID(name, -1);		

	if (proc_name_check(name))
		return -1;

	pid	= pid_get(name);
	if (pid == -1)
		return -1;

	DEBUG("stopping pid:%d\n", pid);
	if (strstr(name, "app")) {
		app_stop(pid);	
	} else {
		DEBUG("pid:%d isn't app, skip\n", pid);
	}

	return 0;
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

	fd	= accept(sk, (struct sockaddr *)&addr, &len);
	if (fd < 0) {
		DEBUG("accept faild\n");
		return;
	}

	//DEBUG("get a call from %s:%d\n", inet_ntoa(addr.sin_addr) \
			, ntohs(addr.sin_port));
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

	sk	= socket_init();
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
