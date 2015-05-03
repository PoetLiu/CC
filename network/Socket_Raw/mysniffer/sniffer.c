/*
 *
 * Authored by Liu Peng on: Thu Apr 23 10:26:11 CST 2015
 *
 * @desc: 使用原始套接字 socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))
 * 	  接收所有发往目的MAC是本机的所有类型的数据帧
 * 	  同时接收从本机发出的所有数据帧
 *
 * @func:
**/
#include "sniffer.h"

int socket_init();
void main_loop(int);
u_int16_t check_sum(u_int16_t*, int);
void sig_int(int);
void sig_quit(int);
char *mac2str(unsigned char *, char *);
char *get_tcp_out(const char *, int, char *, int);
char *get_udp_out(const char *, int, char *, int);
char *get_arp_out(const char *, int, char *, int);

struct proc_module sniffer_module[] = {
	{
		.module_id	= TCP_PARAM,
		.name		= "tcp",
		.proc_func	= get_tcp_out,
	},
	{
		.module_id	= UDP_PARAM,
		.name		= "udp",
		.proc_func	= get_udp_out,
	},
	{
		.module_id	= ARP_PARAM,
		.name		= "arp",
		.proc_func	= get_arp_out,
	},
};

struct sniffer_param sniffer; 

// ip u_int32_t to str version of inet_ntoa
// warning it's not thread safe, due to inet_ntoa() use a static memory
// 如果IP 值为一个整数， 则从该整数转换为IP字符串时， 需要考虑大小端格式
char *ip2str(u_int32_t ip)
{
	struct in_addr addr;
	addr.s_addr = ip;
	return inet_ntoa(addr);
}

// return val:
// 1 little endian
// 0 big endian
int get_byte_order()
{
	u_int16_t i = 0x0100;	
	return *((uchar *)&i) == 0x01;
}

// thread safe
char *ip2str_r(u_int32_t ip, char *buf)
{
	uchar tmp_ip[IP_BIN_LEN];
	memcpy(tmp_ip, (uchar *)&ip, sizeof(tmp_ip));

	if (get_byte_order())
		sprintf(buf, "%d.%d.%d.%d", tmp_ip[3], tmp_ip[2], tmp_ip[1], tmp_ip[0]);
	else
		sprintf(buf, "%d.%d.%d.%d", tmp_ip[0], tmp_ip[1], tmp_ip[2], tmp_ip[3]);
	return buf;
}

// thread safe version of inet_ntoa
char *inet_ntoa_r(struct in_addr addr, char *buf)
{
	return ip2str_r(addr.s_addr, buf);	
}

// ip array to str
// thread safe
char *ipa2str(uchar *ip, char *buf)
{
	return ip2str_r(*((u_int32_t *)ip), buf);	
}

// thread safe
char *mac2str(uchar *mac, char *buf)
{
	uchar tmp_mac[MAC_BIN_LEN];
	
	memcpy(tmp_mac,mac,ETH_ALEN);
	sprintf(buf, MAC_FORMAT_STRING_CAPITAL, MAC_FORMAT_SPLIT(tmp_mac));

	return buf;
}

// 校验和
u_int16_t check_sum(u_int16_t *addr, int len)
{
	register int nleft = len;
	register int sum = 0;
	register u_int16_t *w = addr;
	short answer = 0;
	
	while (nleft > 1) {
	        sum	+= *w++;
	        nleft	-= 2;
	}

	if (nleft == 1) {
	        *(uchar *)(&answer)	= *(uchar *)w;
	        sum	+=answer;
	}
	
	sum	= (sum >> 16) + (sum & 0xffff);
	sum	+= (sum >> 16);
	answer	= ~sum;
	return answer;
}

void sig_int(int sig)
{
	DEBUG("catch a SIGINT\n");
}

void sig_quit(int sig)
{
	struct msg_thread *thread;
	struct msg_thread *p;

	printf("\ncatch a SIGQUIT, program will be exit.\n");
	list_for_each_entry_safe(thread, p, &sniffer.child_threads, list_node) {
		printf("sending cancel signal to %s thread.\n", thread->proc->name);
		pthread_cancel(thread->tid);
		printf("signal send done! watting %s thread exit.\n", thread->proc->name);
		pthread_join(thread->tid, NULL);
		printf("%s thread exit.\n", thread->proc->name);
		destroy_queue(thread->msg_queue);
		list_del(&thread->list_node);
		SAFE_FREE(thread);
	}
	printf("-------------------------------------------\n");
	printf("All child thread exited\n");
	pthread_mutex_destroy(&sniffer.pmutex);
	printf("Package All:%llu tcp:%llu udp:%llu arp:%llu\n", 
		sniffer.all_pkg, sniffer.tcp_pkg, sniffer.udp_pkg, sniffer.arp_pkg);
	exit(0);
}

int socket_init()
{
	int sk = -1;

	// 创建一个TCP的原始套接字
	if (0 > (sk = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL)))) {
		ERROR("socket create faild");
		return -1;
	}

	DEBUG("sock %d\n", sk);
	return sk;
}

// 16:14:38.677364
char *get_time(char *buf, int len)
{
	struct timeval tv;
	struct tm *t;

	gettimeofday(&tv, NULL);
	t	= localtime(&tv.tv_sec);
	snprintf(buf, len, "%02d:%02d:%02d.%06u", t->tm_hour, t->tm_min, 
			t->tm_sec, (unsigned int)tv.tv_usec);
	return buf;
}

char *get_tcp_flag(const struct tcphdr *tcp, char *buf, int len)
{
	int i = 0;

	memset(buf, 0, len);
	if (tcp->syn)	buf[i++] = 'S';
	if (tcp->fin)	buf[i++] = 'F';
	if (tcp->psh)	buf[i++] = 'P';
	if (tcp->rst)	buf[i++] = 'R';
	if (tcp->ack)	buf[i++] = '.';

	i	= i > len ? len : i;
	buf[i]	= '\0';
	return buf;
}

char *get_tcp_out(const char *dbuf, int dlen, char *obuf, int olen)
{
	
	struct ip *iph;
	struct tcphdr *tcp;
	struct ethhdr *eth;
	char src_mac[MAC_STR_LEN] = {0};
	char dst_mac[MAC_STR_LEN] = {0};
	char src_ip[IP_STR_LEN];
	char dst_ip[IP_STR_LEN];
	char time[TIME_STR_LEN] = {0};
	char cflag[10] = {0};
	u_int16_t tcp_dlen, dst_port, src_port;

	eth		= (struct ethhdr*)dbuf;
	iph		= (struct ip*)(dbuf + sizeof(struct ethhdr));
	tcp		= (struct tcphdr*)((char *)iph + sizeof(struct ip));
	tcp_dlen	= ntohs(iph->ip_len) - (iph->ip_hl)*4 - (tcp->doff)*4;
	src_port	= ntohs(tcp->source);
	dst_port	= ntohs(tcp->dest);
	inet_ntoa_r(iph->ip_src, src_ip);
	inet_ntoa_r(iph->ip_dst, dst_ip);
	mac2str(eth->h_source, src_mac);
	mac2str(eth->h_dest, dst_mac);

	if ((src_port == 8080 || dst_port == 8080)) {
#if DISP_MAC == 1
		snprintf(obuf, olen, "%s %s IP %s:%d > %s %s:%d Flags[%s] seq:%u ack:%u win:%d len:%u\n",
			get_time(time, sizeof(time)), src_mac, src_ip, src_port, dst_mac, dst_ip, dst_port, 
			get_tcp_flag(tcp, cflag, sizeof(cflag)), ntohl(tcp->seq), ntohl(tcp->ack_seq),
			ntohs(tcp->window), tcp_dlen);
#else
		snprintf(obuf, olen, "%s IP %s:%d > %s:%d Flags[%s] seq:%u ack:%u win:%d len:%u\n",
			get_time(time, sizeof(time)), src_ip, src_port, dst_ip, dst_port, 
			get_tcp_flag(tcp, cflag, sizeof(cflag)), ntohl(tcp->seq), ntohl(tcp->ack_seq),
			ntohs(tcp->window), tcp_dlen);
#endif
	}

	return obuf;
}

char *get_udp_out(const char *dbuf, int dlen, char *obuf, int olen)
{
	struct ip *iph;
	struct udphdr *udph;
	u_int16_t src_port, dst_port, udp_len;
	char src_ip[IP_STR_LEN], dst_ip[IP_STR_LEN];
	char time[TIME_STR_LEN];

	iph		= (struct ip*)(dbuf + sizeof(struct ethhdr));
	udph		= (struct udphdr*)((char *)iph + sizeof(struct ip));
	src_port	= ntohs(udph->source);
	dst_port	= ntohs(udph->dest);
	udp_len		= ntohs(udph->len) - sizeof(struct udphdr);
	inet_ntoa_r(iph->ip_src, src_ip);
	inet_ntoa_r(iph->ip_dst, dst_ip);

	snprintf(obuf, olen, "%s IP %s.%d > %s.%d UDP length %d\n",
		get_time(time, sizeof(time)), src_ip, src_port, dst_ip, dst_port, udp_len);
	return obuf;
}

char *get_mac_type(char *mac)
{
	char tmp_mac[MAC_STR_LEN] = {0};
	uchar broadcast_bin_mac[MAC_BIN_LEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	mac2str(broadcast_bin_mac, tmp_mac);

	if (memcmp(tmp_mac, mac, MAC_STR_LEN) == 0)
		return "Broadcast";
	else
		return mac;
}

char *get_arp_out(const char *dbuf, int dlen, char *obuf, int olen)
{
	struct ethhdr *eth = (struct ethhdr*)dbuf;
	struct ether_arp *arp = (struct ether_arp*)(dbuf + sizeof(struct ethhdr));
	u_int16_t op = ntohs(arp->ea_hdr.ar_op);
	char dst_mac[MAC_STR_LEN] = {0}, src_mac[MAC_STR_LEN] = {0};
	char esrc_mac[MAC_STR_LEN] = {0}, edst_mac[MAC_STR_LEN] = {0};
	char dst_ip[IP_STR_LEN] = {0}, src_ip[IP_STR_LEN] = {0};
	char time[TIME_STR_LEN] = {0};

	mac2str(arp->arp_tha, dst_mac);
	mac2str(arp->arp_sha, src_mac);
	mac2str(eth->h_source, esrc_mac);
	mac2str(eth->h_dest, edst_mac);
	ipa2str(arp->arp_tpa, dst_ip); 
	ipa2str(arp->arp_spa, src_ip);

	if (op == ARP_REQUEST) 
		snprintf(obuf, olen, "%s ARP, Request who-has %s (%s) tell %s, length %d\n",
			get_time(time, sizeof(time)), dst_ip, get_mac_type(edst_mac), src_ip, 
			dlen - sizeof(struct ethhdr));
	if (op == ARP_REPLY) 
		snprintf(obuf, olen, "%s ARP, Reply %s is-at %s, length %d\n",
			get_time(time, sizeof(time)), src_ip, esrc_mac, dlen - sizeof(struct ethhdr));
	return obuf;
}

void thread_printf(const char *msg)
{
	pthread_mutex_lock(&sniffer.pmutex);
	printf(msg);
	pthread_mutex_unlock(&sniffer.pmutex);
}

Queue *get_queue_by_id(int id)
{
	struct msg_thread *thread;
	struct msg_thread *p;
	list_for_each_entry_safe(thread, p, &sniffer.child_threads, list_node) {
		if (thread->proc->module_id == id)
			return thread->msg_queue;
	}
	return NULL;
}

void * msg_proc_thread(void *thread_arg)
{
	char obuf[OUTPUT_MSG_LEN] = {0};
	char *dbuf = NULL;
	int dlen = 0;
	int cp_flag = 0;
	struct proc_module *proc = (struct proc_module *)thread_arg;
	pthread_t tid = pthread_self();
	Queue *que = get_queue_by_id(proc->module_id);

	DEBUG("create %s proc thread ok tid:%u\n", proc->name, (unsigned int)tid);
	while (1) {
		pthread_testcancel();
		if (de_queue(que, &dbuf, &dlen, cp_flag) == 0) {
			thread_printf(proc->proc_func(dbuf, dlen, obuf, sizeof(obuf)));
			SAFE_FREE(dbuf);
			obuf[0]	= '\0';
		}
		pthread_testcancel();
	}
}

void main_loop(int sk)
{
	char recv_buf[MAX_MSG_BUF_LEN] = {0};
	struct ip *iph;
	struct ethhdr *eth;
	int rlen;
	Queue *tmp;
	
	eth	= (struct ethhdr*)recv_buf;
	iph	= (struct ip*)(recv_buf + sizeof(struct ethhdr));

	while (1) {
		rlen = recvfrom(sk, recv_buf, sizeof(recv_buf), 0, NULL, 0);
		switch (ntohs(eth->h_proto)) {
			case ETHERTYPE_IP:
				switch (iph->ip_p) {
					case IPPROTO_TCP:
						sniffer.tcp_pkg++;
						if ((tmp = get_queue_by_id(TCP_PARAM)) == NULL)
							break;
						en_queue(tmp, recv_buf, rlen);
						break;
					case IPPROTO_UDP:
						sniffer.udp_pkg++;
						if ((tmp = get_queue_by_id(UDP_PARAM)) == NULL)
							break;
						en_queue(tmp, recv_buf, rlen);
						break;
					default:
				//		DEBUG("unkown ip proto:%d\n", iph->ip_p);
						break;
				}
				break;
			case ETHERTYPE_ARP:
				sniffer.arp_pkg++;
				if ((tmp = get_queue_by_id(ARP_PARAM)) == NULL)
					break;
				en_queue(tmp, recv_buf, rlen);
				break;
			default:
				break;
		}
		sniffer.all_pkg++;
		memset(recv_buf, 0, rlen);
	}
}


struct proc_module * get_module_by_name(const char *name)
{
	int i = 0;
	for (i = 0; i < GET_ARRAY_SIZE(sniffer_module); i++) {
		if (strcmp(sniffer_module[i].name, name) == 0)
			return &sniffer_module[i];
	}
	return NULL;
}

int sniffer_init(int *sk, char **module_name, int name_num)
{
	struct msg_thread *tmp = NULL;
	struct proc_module *msg_module = NULL;
	int i = 0;

	INIT_LIST_HEAD(&sniffer.child_threads);
	pthread_mutex_init(&sniffer.pmutex, NULL);
	sniffer.all_pkg	= 0;
	sniffer.tcp_pkg	= 0;
	sniffer.udp_pkg	= 0;
	sniffer.arp_pkg	= 0;


	/* create child thread */
	for (i = 0; i < name_num; i++) {
		if ((msg_module = get_module_by_name(module_name[i])) == NULL) {
			DEBUG("module not exist %s\n", module_name[i]);
			return -1;
		}
		tmp = (struct msg_thread*)malloc(sizeof(struct msg_thread));
		if (!tmp) {
			DEBUG("malloc faild\n");
			return -1;
		}
		memset(tmp, 0, sizeof(struct msg_thread));
		tmp->proc	= msg_module;
		tmp->msg_queue	= init_queue();
		if (pthread_create(&tmp->tid, NULL, msg_proc_thread, msg_module) != 0) {
			DEBUG("create %s thread faild\n", tmp->proc->name);
			return -1;
		}
		list_add(&tmp->list_node, &sniffer.child_threads);
	}
	
	if (0 > (*sk = socket_init())) {
		DEBUG("socket_init faild\n");	
		return -1;
	}

	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, sig_int);
	if (signal(SIGQUIT, SIG_IGN) != SIG_IGN)
		signal(SIGQUIT, sig_quit);

	// 原始套接字需要root权限
	setuid(getpid());
	return 0;
}

// TODO ip ip6 显示
int main(int argc, char **argv)
{
	int sock_fd;

	if (argc < 2) {
		printf("Usage %s <tcp|udp|arp>\n", argv[0]);
		return -1;
	}
	
	if (sniffer_init(&sock_fd, argv+1, argc-1) != 0) {
		DEBUG("init faild\n");
		return -1;
	}
		
	DEBUG("init ok!\n");
	main_loop(sock_fd);	

	return 0;
}

