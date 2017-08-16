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
#include "mytcp.h"

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
	char recv_buf[128] = {0};
	struct ip *ip;
	struct tcphdr *tcp, *rtcp;
	struct psd_header *ph_tcp; // tcp 伪首部
	int ip_len, rlen;
	struct sockaddr_in src;

	inet_aton("127.0.0.1", &src.sin_addr);
	// 我们的tcp报文中没有Data字段， 所以ip报文长度如下
	ip_len	= sizeof(struct ip) + sizeof(struct tcphdr);
	ip	= (struct ip *)buf;
	tcp	= (struct tcphdr *)(buf + sizeof(struct ip));
	ph_tcp	= (struct psd_header *)check_sum_buf;

	rtcp	= (struct tcphdr *)(recv_buf + sizeof(struct ip));

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
	ip->ip_src	= src.sin_addr; 

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
	// 校验和需要
	ph_tcp->daddr	= target->sin_addr.s_addr;
	ph_tcp->mbz	= 0;
	ph_tcp->ptcl	= IPPROTO_TCP;
	ph_tcp->tcpl	= htons(sizeof(struct tcphdr));
	ph_tcp->saddr	= ip->ip_src.s_addr;

	/* 进行TCP三次握手过程
	 * 1. 发送SYN
	 * 	SYN=1 seq=x
	 * 2. 接收SYN+ACK包
	 * 	SYN=1 ACK=1 seq=y ack_seq=x+1
	 * 3. 发送ACK包
	 * 	ACK=1 ack_seq=y+1
	 *
	 *----------------------------------------------------------------
	 *注意：这里的操作步骤无法完成三次握手
	 *原因：使用原始套接字接收ip报文(处于网络层), 当完成三次握手第一步之后
	 *	服务器将会回复一个SYN+ACK包， 这个包被本程序接收， 同时也会传递
	 *	到运输层(TCP).由于进行第一步时， 没有通过TCP(运输层)注册， 所以
	 *	运输层没有这个SYN的记录。所以当运输层接收到这个“不请自来”的SYN+ACK
	 *	时， 会发送一个RST报文回复， 重置该连接。所以这里进行到第二步之后
	 *	运输层会回复一个RST， 在我们的程序发送ACK包之前。
	 *tcpdump:
	 *	21:42:57.953128 IP localhost.8888 > localhost.http-alt: Flags [S], seq 30000, win 10052, length 0
	 *	21:42:57.953149 IP localhost.http-alt > localhost.8888: Flags [S.], seq 868530548, ack 30001, 
	 *						win 32792, options [mss 16396], length 0
	 *	21:42:57.953158 IP localhost.8888 > localhost.http-alt: Flags [R], seq 30001, win 0, length 0
	 *	21:42:57.956669 IP localhost.8888 > localhost.http-alt: Flags [.], ack 1, win 10052, length 0
	 *	第三个报文即为RST包， 非本程序发送。查看最左边的时间序列， 在收到SYN+ACK极短的时间内
	 *	回复了RST
	 *
	 * */
	while (1) {
		// 经测试， 这里的check可以为任何值, 即错误的校验码服务端也可收到
		// 查询资料发现， 可能开启了网卡硬件校验和
		memcpy(check_sum_buf + sizeof(struct psd_header), tcp, sizeof(struct tcphdr)); 
		tcp->check	= check_sum((unsigned short *)check_sum_buf, 
					sizeof(struct psd_header) + sizeof(struct tcphdr));
		DEBUG("send syn %d\n", ip_len);
		sendto(sk, buf, ip_len, 0, (struct sockaddr *)target, sizeof(struct sockaddr_in));
	
		/*
		 * 由于这里会接收到所有协议为TCP的IP包， 需要判断包的目的端口和地址
		 * 当包的目的、源符合要求时， 解析
		 * */
		while (1) {
			DEBUG("watting for reply syn+ack\n");
			rlen = recvfrom(sk, recv_buf, sizeof(recv_buf), 0, NULL, 0);
			DEBUG("recv %d\n", rlen);
			DEBUG("send seq %u send ack_seq %u recv seq %u recv ack_seq %u\n", 
					ntohl(tcp->seq), ntohl(tcp->ack_seq), ntohl(rtcp->seq), ntohl(rtcp->ack_seq));

			if (rlen < 40) {
				DEBUG("msg too short\n");
				continue;	
			} else if (ntohs(rtcp->dest) != srcport) {
				DEBUG("other port %d\n", ntohs(rtcp->dest));
				continue;
			} else if (rtcp->rst && rtcp->ack) {
				DEBUG("dest port shutdown\n");
				continue;
			} else if (rtcp->ack && rtcp->syn) {
				DEBUG("!!!!get server msg\n");
				tcp->ack_seq	= htonl(ntohl(rtcp->seq) + 1);
				tcp->ack	= 1;
				tcp->syn	= 0;
				tcp->seq	= 0;

				sendto(sk, buf, ip_len, 0, (struct sockaddr *)target, sizeof(struct sockaddr_in));
				DEBUG("send seq %u send ack_seq %u recv seq %u recv ack_seq %u send ack %d\n", 
					ntohl(tcp->seq), ntohl(tcp->ack_seq), ntohl(rtcp->seq), ntohl(rtcp->ack_seq), 
					tcp->ack);
				DEBUG("waitting for connected\n");
				sleep(30);
				exit(0);
			} else {
				DEBUG("unkown\n");
			}
			
		}
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
