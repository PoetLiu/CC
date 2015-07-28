#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <sys/sysinfo.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <setjmp.h>
#include <mntent.h>
#include <netdb.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <errno.h>
#include <regex.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "nc_ipc.h"
#include "nt_kernel.h"
#include "nc_netlink.h"
#include "igd/igd_libprocess.h"
#include "ruijie.h"

#define RUIJIE_DEBUG(fmt, args...) do{if(1)console_printf("\n\nDEBUG:%s:%d "fmt, __FUNCTION__, __LINE__, ##args);}while(0)
#define RUIJIE_ERROR(fmt, args...) do{if(1)console_printf("\n\nERROR:%s:%d "fmt, __FUNCTION__, __LINE__, ##args);}while(0)


//ruijie
static will_start = 0;
static struct ruijie_param ruijie;
static struct ruijie_logs logs;
static char *ruijie_cfg = "/tmp/ruijie.conf";
static char dev_name[IFNAMSIZ];
static char *dev_info = "";

static char *ruijie_path = RUIJIE_APP_PATH"/bin/ruijieclient";
static pid_t ruijie_pid = 0;

static u_int32_t jiffies = 0;

static int ruijie_init(void);
static int ruijie_is_run();

static int ruijie_will_start(void);
static int ruijie_start(void);
static int ruijie_stop(void);
static int ruijie_restart();
static int ruijie_param_set(struct ruijie_param *);
static int ruijie_param_get(struct ruijie_param *);

static int ruijie_saveparam();
static void log_add(const char *, ...);

static	char * print_ipad(__be32 ipaddr)
{
	static char ipreturn[20];
	struct l2b {
	   union {
	   __be32 iplong;
	   unsigned char ipchar[4];
	   }ip;
	} ip;
	memset(ipreturn, 0 ,sizeof(ipreturn));
	ip.ip.iplong = ipaddr;
	snprintf(ipreturn,16, "%u.%u.%u.%u",
		ip.ip.ipchar[0],
		ip.ip.ipchar[1],
		ip.ip.ipchar[2],
		ip.ip.ipchar[3]);

	return ipreturn;
}

static char * parse_ipad(
	ip_t * ipout,        /* pointer to IP address to set */
	unsigned * sbits,       /* default subnet bit number */
	char    * stringin      /* buffer with ascii to parse */
	)
{
	char * cp;
	int dots = 0;   /* periods imbedded in input string */
	int number;
	union{
		unsigned char c[4];
		unsigned long l;
	} retval;
	char * toobig = "each number must be less than 255";
	char * errorip = "each number must be more than 0";
	
	cp = stringin;
	while(*cp)
	{
		if(*cp > '9' || *cp < '.' || *cp == '/')
			return("all chars must be digits (0-9) or dots (.)");
		if(*cp == '.')  dots++;
		cp++;
	}

	if( dots != 3 )
		return("string must contain 3 dots (.)");

	cp = stringin;
	if((number = atoi(cp)) > 255)   /* set net number */
		return(toobig);
	if(number<0)
		return(errorip);
	
	retval.c[0] = (unsigned char)number;

	while(*cp != '.')cp++;      /* find dot (end of number) */
	cp++;                       /* point past dot */

	if(dots == 1 || dots == 2) 
		retval.c[1] = 0;
	else
	{
		number = atoi(cp);
		while(*cp != '.')cp++;   /* find dot (end of number) */
		cp++;            /* point past dot */
		if(number > 255) return(toobig);
		retval.c[1] = (unsigned char)number;
	}

	if(dots == 1) retval.c[2] = 0;
	else
	{
	       number = atoi(cp);
	       while(*cp != '.')cp++;   /* find dot (end of number) */
	       cp++;            /* point past dot */
	       if(number > 255) return(toobig);
	       retval.c[2] = (unsigned char)number;
	}
	
	if((number = atoi(cp)) >255)
	       return(toobig);
	retval.c[3] = (unsigned char)number;
	
	if(retval.c[0] < 128) *sbits = 8;
	else if(retval.c[0] < 192) *sbits = 16;
	else *sbits = 24;
	
	*ipout = retval.l;      /* everything went OK, return number */
	return(NULL);         /* return OK code (no error string) */
}

char *
get_cgi_env(const char *varname)
{
	char *result;

	result = getenv(varname);
	return result;
}

static int _ruijie_kill()
{

	if( ruijie_pid > 0 ){
		sleep(1);
		if( kill(ruijie_pid, SIGKILL) == 0){
			RUIJIE_DEBUG("kill ok:pid=%d, sig=%d\n", ruijie_pid, SIGTERM);
			system("killall ruijieclient");
			ps_wait(ruijie_pid, NULL);
			ruijie_pid = 0;
			return 0;
		}
		else
			RUIJIE_ERROR("kill ruijieclient:pid=%d\n", ruijie_pid);
	}
	else
		RUIJIE_ERROR("pid=%d\n", ruijie_pid);

	return -1;
}

static int ruijie_init(void)
{
	struct nc_if *ifc = NULL, *tmp = NULL;
	
	logs.log_num = 0;
	INIT_LIST_HEAD(&logs.list);

	ifc = nc_get_if(IF_TYPE_WAN);
	if (!ifc) {
		RUIJIE_ERROR("nc_get_if failed\n");
		return NRET_FALSE;
	}

	setenv("LD_LIBRARY_PATH", RUIJIE_APP_PATH"/bin", 1);
	mkdir("/tmp/ruijie", 0777);
	mkdir("/tmp/ruijie/private", 0777);
	mkdir("/tmp/ruijie/var", 0777);
	mkdir("/tmp/ruijie/var/locks", 0777);

	ruijie.enable = 1;
	memset(dev_name, 0, sizeof(dev_name));
	igd_list_for_each(0, IGD_INTERFACE_WAN_NUM) {
		tmp = ifc + i;
		if (!tmp->enable || tmp->offline)
			continue;
		strncpy(dev_name, tmp->devname, sizeof(dev_name));
		RUIJIE_DEBUG("using %s to auth\n", dev_name);
		log_add("WAN口网卡设备名:%s", dev_name);
	} igd_list_for_each_end();
}

static int ruijie_is_run()
{
	return ruijie_pid;
}

static ruijie_will_start()
{
	will_start = 1;
}

static int ruijie_start(void)
{
	FILE *f;
	char cmdline[IGD_MIN_BUFSZ] = {0};
	
	if (dev_name[0]) {
		sprintf(cmdline, "%s -n %s -u %s -p %s --dhcpmode %d", ruijie_path, dev_name, ruijie.user, ruijie.pass, ruijie.dhcp_mode);
		ruijie_pid	= ps_start(cmdline);
		RUIJIE_DEBUG("ruijie start ok, pid=%d\n", ruijie_pid);
		log_add("认证进程已启动, pid:%d", ruijie_pid);
	} else {
		RUIJIE_ERROR("can not found wan interface, please check wan\n");	
		log_add("未找到合适的网卡设备, 请检查WAN口");
		return NRET_FALSE;
	}

	return NRET_TRUE;
}

static int ruijie_stop(void)
{
	if( ruijie_is_run() )
		_ruijie_kill();

	return NRET_TRUE;
}

static int ruijie_restart()
{
	ruijie_stop();
	if( ruijie.enable ) 
		ruijie_start();

	return NRET_TRUE;
}

static int ruijie_loadparam()
{
	cJSON * json;
	char *p_user, *p_pass;
	int enable;
	
	json = json_load_file(RUIJIE_APPCFG_PATH);
	if (!json) {
		RUIJIE_ERROR("load %s failed\n", RUIJIE_APPCFG_PATH);
		return -1;
	}

	//enable
	ruijie.enable		= cJSON_GetDigitValue(json, "enable");
	ruijie.dhcp_mode	= cJSON_GetDigitValue(json, "dhcp_mode");
	ruijie.broadcast_mode	= cJSON_GetDigitValue(json, "broadcast_mode");
	p_user			= cJSON_GetStringValue(json, "user");
	p_pass			= cJSON_GetStringValue(json, "pass");

	strncpy(ruijie.user, p_user, sizeof(ruijie.user));
	strncpy(ruijie.pass, p_pass, sizeof(ruijie.pass));

	if (!ruijie.dhcp_mode) {
		int subnet;
		char *p_ip = NULL, *p_mask = NULL, *p_gateway = NULL, *p_mac = NULL;
		p_ip		= cJSON_GetStringValue(json, "ip");
		p_mask		= cJSON_GetStringValue(json, "mask");
		p_gateway	= cJSON_GetStringValue(json, "gateway");
		p_mac		= cJSON_GetStringValue(json, "mac");
		
		if (!p_ip || !p_mask || !p_gateway || !p_mac) {
			RUIJIE_ERROR("NULL param !\n");
			goto end;
		}
		parse_ipad(&ruijie.ip.s_addr, &subnet, p_ip);
		parse_ipad(&ruijie.mask.s_addr, &subnet, p_mask);
		parse_ipad(&ruijie.gateway.s_addr, &subnet, p_gateway);
		str2mac(p_mac, &ruijie.mac);
	}

end:
	cJSON_Delete(json);

	return 0;
}

static int ruijie_saveparam()
{
	cJSON * json ;

	RUIJIE_DEBUG("\n");

	json = cJSON_CreateObject();
	if (!json) {
		RUIJIE_ERROR("create json failed\n");
		return -1;
	}

	cJSON_AddNumberToObject(json, "enable", ruijie.enable);
	cJSON_AddNumberToObject(json, "dhcp_mode", ruijie.dhcp_mode);
	cJSON_AddNumberToObject(json, "broadcast_mode", ruijie.broadcast_mode);
	cJSON_AddStringToObject(json, "user", ruijie.user);
	cJSON_AddStringToObject(json, "pass", ruijie.pass);
	cJSON_AddStringToObject(json, "mac", mac2str(ruijie.mac));

	// if we use static ip
	if (!ruijie.dhcp_mode) {
		cJSON_AddStringToObject(json, "ip", print_ipad(ruijie.ip.s_addr));
		cJSON_AddStringToObject(json, "mask", print_ipad(ruijie.mask.s_addr));
		cJSON_AddStringToObject(json, "gateway", print_ipad(ruijie.gateway.s_addr));
	}
	json_dump_file(json, RUIJIE_APPCFG_PATH, 1);

	cJSON_Delete(json);
	return 0;
}

struct nc_if *nc_wanuiname2if(char *name)
{
	struct nc_if *ifc;
	struct nc_if *org;

	org = nc_get_if(IF_TYPE_WAN);
	if (!org)
		return NULL;

	igd_list_for_each(0, IF_MX) {
		ifc = org + i;
		if (strcmp(ifc->uiname, name)) 
			continue;
		return ifc;
	} igd_list_for_each_end();
	return NULL;
}

static int _events(struct igd_netlink_handler * nlh)
{
	char buf[8192] = {0};
	struct igd_nlk_comm * comm;	
	msg_app_t * app_msg;
	struct dev_notify * dev_msg;
	struct nlk_if_msg * if_msg;

	memset(buf, 0, sizeof(buf));
	nlk_msg_recv(nlh, buf, sizeof(buf));

	comm = (struct igd_nlk_comm *)&buf[0];

	if (comm->gid == NLKMSG_GRP_STOP) {
		RUIJIE_DEBUG("app events\n");
		app_msg = (msg_app_t *)&buf[0];

		if (app_msg->comm.key == MSG_STOP) {
			RUIJIE_DEBUG("app events: stop\n");
			ruijie_stop();
			exit(0);
		}

		RUIJIE_DEBUG("app events: unknown\n");
	}

	if (comm->gid == NLKMSG_GRP_IF) {
		int offset = 0, i = 0;
		if_msg = (struct nlk_if_msg *)&buf[0];

		RUIJIE_DEBUG("key%d :action:%d -%s-%s\n", if_msg->comm.key, if_msg->comm.action, if_msg->devname, if_msg->uiname);
		if (if_msg->type != IF_TYPE_WAN)
			return 0;

		if (if_msg->comm.action == IGD_NLK_DEL) {
			if (!strcasecmp(if_msg->devname, dev_name)) {
				log_add("wan口已断开");
				RUIJIE_ERROR("wan port down, please check your wire\n");
			}
		} else if (if_msg->comm.action == IGD_NLK_ADD) {
			if (!strcasecmp(if_msg->devname, dev_name)) {
				log_add("wan口已连接");
				RUIJIE_ERROR("wan port up, restarting ruijie auth\n");
			}
		}
	}

	return 0;
}

static int ruijie_wan_get(struct ruijie_param *p)
{
	struct nos_wan_cfg wan;
	struct nc_if *ifc = NULL;
	int old_dhcp_mode	= ruijie.dhcp_mode;
	ip_t	host_ip;

	if (!p)
		return -1;

	wan.wanid	= 1;
	if (get_wan_config(&wan) != NRET_TRUE) {
		RUIJIE_DEBUG("get_wan_config faild, check param\n");
		return NRET_FALSE;
	}
	p->ip.s_addr		= wan.ip.s_addr;		
	p->mask.s_addr		= wan.mask.s_addr;
	p->gateway.s_addr	= wan.gw.s_addr;		
	p->dns[0].s_addr	= wan.manu_dns[0].s_addr;
	p->dns[1].s_addr	= wan.manu_dns[1].s_addr;
	memcpy(p->mac, wan.mac, MAC_SIZE);
	if (old_dhcp_mode) {
		if (wan.connect_type == WAN_CONN_TYPE_STATIC)
			ruijie.dhcp_mode	= DHCP_MODE_DISABLE;
	} else {
		if (wan.connect_type == WAN_CONN_TYPE_DHCP)
			ruijie.dhcp_mode	= DHCP_MODE_BEFORE_AUTH;
	}

	if (!(ifc = nc_wanuiname2if("WAN1"))) {
		RUIJIE_DEBUG("get WAN1 ifc faild\n");
		return NRET_FALSE;
	}
	memcpy(p->mac_default, ifc->mac_default, MAC_SIZE);

	// get clone mac
	if (p->host_ip.s_addr) {
		scall_param sw_pa = {0}, sw_pb = {0}, sw_pc = {0};
		struct in_addr hostip;
		hostip.s_addr	= p->host_ip.s_addr;
		RUIJIE_DEBUG("IGD_CONNECTIP = %s\n", inet_ntoa(hostip));
		client_SwitchCall(IGD_MSG_HOST_IP2MAC, PRM_VALUE(sw_pa, p->host_ip.s_addr), 
				PRM_POINTER(sw_pb, p->mac_clone, MAC_SIZE), 
				PRM_VALUE_NULL(sw_pc));
	} else {
		RUIJIE_DEBUG("get IGD_CONNECTIP faild\n");
	}


	return NRET_TRUE;
}


static int ruijie_param_get(struct ruijie_param *p)
{
	if (!p)
		return NRET_FALSE;

	// get wan cfg param
	ruijie_wan_get(p);

	// get common cfg
	strncpy(p->user, ruijie.user, sizeof(ruijie.user));
	strncpy(p->pass, ruijie.pass, sizeof(ruijie.pass));
	p->enable		= ruijie.enable;
	p->broadcast_mode	= ruijie.broadcast_mode;
	p->dhcp_mode		= ruijie.dhcp_mode;

	return NRET_TRUE;
}

static int ruijie_wan_set(struct ruijie_param *p)
{

	struct nos_wan_cfg wan;

	wan.wanid		= 1;	// wan1
	wan.nat_mode		= 1;	// work_mode:NAT
	memcpy(wan.mac, p->mac, MAC_SIZE);

	if (p->dhcp_mode) {
		wan.connect_type	= WAN_CONN_TYPE_DHCP;
	} else {
		wan.connect_type	= WAN_CONN_TYPE_STATIC;
		wan.ip.s_addr		= p->ip.s_addr;
		wan.mask.s_addr		= p->mask.s_addr;
		wan.gw.s_addr		= p->gateway.s_addr;
		wan.manu_dns[0].s_addr	= p->dns[0].s_addr;
		wan.manu_dns[1].s_addr	= p->dns[1].s_addr;
	}

	if (set_wan_config(&wan) != NRET_TRUE) {
		RUIJIE_DEBUG("set_wan_config faild, check param\n");
		return NRET_FALSE;
	}
	return NRET_TRUE;
}

static int ruijie_param_set(struct ruijie_param *p)
{
	int ret = -1;

	if (!p)
		return NRET_FALSE;

	memcpy(&ruijie, p, sizeof(struct ruijie_param));	
	if ((ret = ruijie_wan_set(&ruijie)) != NRET_TRUE) {
		RUIJIE_DEBUG("wan set faild check param!\n");
		return NRET_FALSE;
	}

	ruijie_restart();
	ruijie_saveparam();

	return NRET_TRUE;
}

// 2015-05-17 16:14:38
char *get_time(char *buf, int len)
{
        struct timeval tv;
        struct tm *t;

        gettimeofday(&tv, NULL);
        t       = localtime(&tv.tv_sec);
        snprintf(buf, len, "%04d-%02d-%02d %02d:%02d:%02d", 
			t->tm_year + 1900, t->tm_mon, t->tm_mday, 
			t->tm_hour, t->tm_min, t->tm_sec);
        return buf;
}

static void log_add(const char * fmt, ...)
{
	struct ruijie_log * l;
	va_list ap;

	if (logs.log_num >= RUIJIE_LOG_NUM_MAX) {
		RUIJIE_DEBUG("log num is max\n");
		l = (struct ruijie_log *)logs.list.prev;
		list_del(&l->list);
	} else {
		l = (struct ruijie_log *)malloc(sizeof(struct ruijie_log));
		if (!l) {
			RUIJIE_ERROR("alloc log failed\n");
			return;
		}
		logs.log_num ++;
	}

	memset(l, 0, sizeof(struct ruijie_log));

	get_time(l->time, RUIJIE_LOG_TIME_LEN);

	va_start(ap, fmt);
	vsprintf(l->log, fmt, ap);
	va_end(ap);

	list_add(&l->list, &logs.list);
}

static int ruijie_log_add(const char *log)
{
	struct ruijie_log * l;

	if (logs.log_num >= RUIJIE_LOG_NUM_MAX) {
		l = (struct ruijie_log *)logs.list.prev;
		list_del(&l->list);
	} else {
		l = (struct ruijie_log *)malloc(sizeof(struct ruijie_log));
		if (!l) {
			RUIJIE_ERROR("alloc log failed\n");
			return;
		}
		logs.log_num ++;
	}

	memset(l, 0, sizeof(struct ruijie_log));

	get_time(l->time, RUIJIE_LOG_TIME_LEN);
	snprintf(l->log, RUIJIE_LOG_MSG_LEN, "%s", log);

	list_add(&l->list, &logs.list);
}

struct ruijie_log * log_next(int index)
{
	int i;
	struct ruijie_log *l, *p;

	i = 0;

	if (!logs.log_num) {
		RUIJIE_DEBUG("empty logs\n");
		return NULL;
	}

	list_for_each_entry_safe(l, p, &logs.list, list) {
		if ( i == index)
			return l;
		i++;
	}

	return NULL;
}

static int ruijie_log_next(int index, struct ruijie_log *log)
{
	struct ruijie_log * l;

	l = log_next(index);
	if (l) {
		memcpy(log, l, sizeof(struct ruijie_log));
		return NRET_TRUE;
	}
	return NRET_FALSE;
}

static int ruijie_log_clean()
{
	struct ruijie_log *l, *p;

	if (!logs.log_num)		// if it's empty already
		return NRET_TRUE;

	list_for_each_entry_safe(l, p, &logs.list, list) {
		list_del(&l->list);
		free(l);
		logs.log_num --;
	}

	INIT_LIST_HEAD(&logs.list);
	RUIJIE_DEBUG("all logs cleaned \n");
	return NRET_TRUE;
}

static int ruijie_call(int msgid, unsigned long ulpa, unsigned long ulpb, void * pv)
{
	RUIJIE_DEBUG("msgid=%d\n", msgid);
	switch(msgid){
		case RUIJIE_PARAM_SET:
			return ruijie_param_set((struct ruijie_param *)ulpa);
		case RUIJIE_PARAM_GET:
			return ruijie_param_get((struct ruijie_param *)ulpa);
		case RUIJIE_LOG_NEXT:
			return ruijie_log_next((int)ulpa, (struct ruijie_log *)ulpb);
		case RUIJIE_LOG_ADD:
			return ruijie_log_add((char *)ulpa);
		case RUIJIE_LOG_CLEAN:
			return ruijie_log_clean();
		default:
			break;
	}

	return NRET_FALSE;
}

int main(int argc, const char *argv[])
{
	struct igd_netlink_handler nlh;
	int max_fd;
	fd_set fds;
	struct timeval tv;
	int ret = -1;

	ruijie_init();
	ruijie_loadparam();
	ruijie_will_start();

	nlk_msg_init(&nlh,
			(0x1 << (NLKMSG_GRP_STOP - 1))	|
			(0x1 << (NLKMSG_GRP_IF - 1))	|
			(0x1 << (NLKMSG_GRP_DEVICE - 1)));
	nt_ipcserver_init2(IPC_PATH_RUIJIE, ruijie_call);

	nlk_start_msg(MSG_START_OK, NULL);

	while(1) {
		tv.tv_sec	= 0;
		tv.tv_usec	= 100*1000;

		FD_ZERO(&fds);
		IGD_FD_SET(nlh.sock_fd, &fds);
		IGD_FD_SET(g_ipcserver_sock, &fds);

		if ((ret = select(max_fd + 1, &fds, NULL, NULL, &tv)) < 0) {
			if (errno == EINTR || errno == EAGAIN)
			    continue;
		}

		if (ret == 0) {
			pid_t pid;
			int status;

			jiffies ++;

			//ruijie
			pid = waitpid(ruijie_pid, &status, WNOHANG);
			if (pid > 0) {
				//ruijie stoped
				ruijie_pid	= 0;
				RUIJIE_DEBUG("ruijieclient stoped\n");
				log_add("锐捷认证已停止");
			}

			if (will_start) {
				if (ruijie.enable)
					ruijie_start();
				will_start	= 0;
			}
			continue;
		}

		if (FD_ISSET(nlh.sock_fd, &fds))
			_events(&nlh);

		if (FD_ISSET(g_ipcserver_sock, &fds))
			ipc_server_accept2();

	}
	return 0;
}
