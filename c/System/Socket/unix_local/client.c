/*
   *
   * Authored by Liu Peng on: Tue Aug 12 18:16:27 CST 2014
   *
   * @desc:基于Unix域的TCP/IP客户端
   *
   * @func:实现客户端->服务器消息发送
   */

#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "unix_local.h"
#include <sys/un.h>
#include <errno.h>

/*
   * Funcion	: 创建一个Unix协议的socket，并通过Unix域协议的文件路径和服务器建立连接
   *
   * Author	: Liu Peng
   *
   * arguments	: Unix域协议文件路径
   * return	: ERROR_CREAT_SOCKET 创建socket失败
   *		  ERROR_CONNECT_SOCKET 连接失败
   */
int comm_connect(char *path)
{
	int sock, len, rv;
	struct sockaddr_un addr;

	if ((sock = socket (AF_UNIX, SOCK_STREAM, 0)) < 0 ) {
		printf("error creat socket!\n");
		return ERROR_CREAT_SOCKET;
	}

	memset (&addr, 0, sizeof (struct sockaddr_un));
	addr.sun_family	= AF_UNIX;
	strcpy (addr.sun_path, path);

	if (( rv = connect (sock, (struct sockaddr *) &addr, sizeof(addr))) < 0) {
		printf("error connect socket!\n");
		close(sock);
		return ERROR_CONNECT_SOCKET;
	}else {
		printf("connect success, socket:%d\n", sock);
	}

	return sock;
}

// 读取命令行内容并发送到服务器
int main()
{

	int sock;
	char str_send[MAX_STR_SEND];

	printf("client:");
	
	// 初始化客户端
	sock	= comm_connect(IPC_PATH);

	if (sock >= 0) {
		printf("client:");
		while (fgets(str_send, MAX_STR_SEND, stdin) != NULL) {
			write (sock, str_send, strlen(str_send));
			printf("client:");
			if(str_send[0] == 'q')
				break;
		}
		printf("error no :%d\n", errno);
	}
	close(sock);
	return 0;
}
