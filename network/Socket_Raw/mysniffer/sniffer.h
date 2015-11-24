#ifndef __MDOS_H__
#define __MDOS_H__

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <sys/time.h>
#include <time.h>
#include <stddef.h>
#include <signal.h>
#include <pthread.h>
#include "linux_list.h"
#include "queue.h"
#include "debug.h"


#ifndef __DISP_ALL
#define DISP_MAC 0
#else
#define DISP_MAC 1
#endif

#define MAC_STR_LEN	18
#define MAC_BIN_LEN	6
#define IP_STR_LEN	16
#define IP_BIN_LEN	4
#define TIME_STR_LEN	16
#define MAX_MSG_BUF_LEN	65535
#define OUTPUT_MSG_LEN  2048

#define MAC_FORMAT_STRING_CAPITAL	"%02x:%02x:%02x:%02x:%02x:%02x"
#define TO_UCHAR(p)			(*(unsigned char*)(p))
#define MAC_FORMAT_SPLIT(mac)		TO_UCHAR(mac),TO_UCHAR(mac+1),TO_UCHAR(mac+2),TO_UCHAR(mac+3),TO_UCHAR(mac+4),TO_UCHAR(mac+5)




#define GET_ARRAY_SIZE(array) 		(sizeof(array)/sizeof(array[0]))

typedef unsigned char uchar;

enum {
	ARP_REQUEST = 1,
	ARP_REPLY,
	RARP_REQUEST,
	RARP_REPLY
};

enum PARAM_ID{
	TCP_PARAM,			// TCP参数
	UDP_PARAM,			// UDP参数
	ARP_PARAM,			// ARP参数
};

// 处理模块：TCP、UDP、ARP处理
struct proc_module {
	int	module_id;					// 模块id
	char	*name;						// 模块名字
	char * (*proc_func)(const char *, int, char *, int);	// 模块处理函数
};

// TCP 伪首部 为了校验和
struct psd_header{
	unsigned long saddr;		// 源地址
	unsigned long daddr;		// 目的地址
	char	mbz;			// 置空
	char	ptcl;			// 协议类型
	unsigned short	tcpl;		// tcp 长度
};

struct msg_thread {
	struct list_head list_node;	// 是struct sniffer_param 中child_threads 链表结点
	pthread_t	tid;		// 线程ID
	Queue		*msg_queue;	// 线程使用的消息队列
	struct proc_module *proc;	// 线程使用的消息处理模块
};

struct sniffer_param {
	struct list_head child_threads; // 所有子线程列表
	pthread_mutex_t pmutex;		// 向屏幕输出消息时使用， 线程同步
	unsigned long long all_pkg;	// 所有数据包的数量
	unsigned long long tcp_pkg;	// tcp报文数量
	unsigned long long udp_pkg;	// udp报文数量
	unsigned long long arp_pkg;	// arp报文数量
};

#endif // def __MDOS_H__

