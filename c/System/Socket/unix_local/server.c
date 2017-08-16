/*
   *
   * Authored by Liu Peng on: Tue Aug 12 17:52:49 CST 2014
   *
   * @desc: 基于unix域的TCP/IP服务器程序，可接收客户端消息
   *
   * @func: 使用Unix域协议，建立TCP/IP服务器
   * 
**/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include "unix_local.h"
#include <errno.h>

/*
   * Funcion	:初始化unix域服务器，创建socket并监听
   *
   * Author	:Liu Peng
   *
   * arguments	:Unix域文件路径
   * return	:ERROR_NULL_PATH 路径名为NULL
   *		 ERROR_CREAT_SOCKET 创建socket失败
   *		 ERROR_BIND_SOCKET 绑定socket失败
   *		 ERROR_LISTEN_SOCKET 监听socket失败
   */
int unix_local_server_init (char *path)
{
	int sock;
	struct sockaddr_un addr;
	sock	= 0;
	memset (&addr, 0, sizeof(addr));

	if (!path)
		return ERROR_NULL_PATH;

	sock = socket (AF_UNIX, SOCK_STREAM, 0);
	if (sock < 0) {
		printf ("error creat unix socket\n");
		return ERROR_CREAT_SOCKET;
	}

	unlink(path);
	addr.sun_family = AF_UNIX;
	strcpy (addr.sun_path, path);

	if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		printf("bind socket error..\n");
		close(sock);
		return ERROR_BIND_SOCKET;
	}

	if (listen(sock, 5) < 0 ) {
		printf("listen on socket error \n");
		close(sock);
		return ERROR_LISTEN_SOCKET;
	}else
		printf("listen on socket\n");

	return sock;
}

/*
   * Funcion	: 处理socket上消息
   * Author	: Liu Peng
   * arguments	: socket文件描述符
   * return	: ERROR_ACCEPT_SOCKET 调用accept函数失败
   *
   */

int comm_proc (int so)
{

	struct sockaddr_un addr;
	int n, counter;
	socklen_t len;
	int read_sock;
	char str_read[MAX_STR_READ];

	len		= sizeof(addr);
	read_sock	= accept(so, (struct sockaddr *)&addr, &len);
	memset(str_read, 0, sizeof(str_read));
	if (read_sock < 0) {
		printf("error accept socket, errno:%d, read_sock:%d\n", errno, read_sock);
		close (read_sock);
		return ERROR_ACCEPT_SOCKET;
	}

	// 注意：这里的n值不知真实从socket读到的字节数，由于网络协议和内核缓存机制
	// 这里的n值往往要小于真实读到的字节数
	while(n = read(read_sock, str_read, MAX_STR_READ) > 0){	
		if( n == 1  && str_read[0] == 'q' ) {
			printf("client quit!\n");
			break;
		}

		// 在客户端发送的消息总是以'\n'结束的情况下
		// 将'\n'替换为'\0'否则将会导致字符串没有结尾
		for(counter = 0; str_read[counter] != '\n'; counter++);
		str_read[counter] = '\0';
		
		printf("client:%s\n", str_read);
	}

	close(read_sock);

}

int main(void)
{
	int sock_fd;
	fd_ et fds;
	struct timeval tv;

	if ((sock_fd = unix_local_server_init(IPC_PATH)) < 0) { 
		printf("server init error, error code:%d\n", sock_fd);
	}else
		printf("server init success\n");
	
	while (1) {
	
		FD_ZERO(&fds);
		FD_SET(sock_fd, &fds);

		tv.tv_sec	= 3;
		tv.tv_usec	= 0;
		
		printf("waitting for client connect!\n");
		if (select(sock_fd + 1, &fds, NULL, NULL, &tv) < 0) {
			if (errno == EINTR || errno == EAGAIN)
				continue;
		}

		if (FD_ISSET(sock_fd, &fds)) {
			printf("process message!\n");
			comm_proc(sock_fd);
		}

	}
	return 0;

}
