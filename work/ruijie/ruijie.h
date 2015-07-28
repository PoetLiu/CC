#ifndef RUIJIE_H 
#define RUIJIE_H 
#include "nc_ipc.h"
#include "igd/igd_interface.h"
#include "linux_list.h"

#define IPC_PATH_RUIJIE 	"/tmp/ruijie_ipc"
#define RUIJIE_APP_PATH 	"/app/ruijie"
#define RUIJIE_APPCFG_PATH 	"/app/app_config/ruijie/config"

#define MAC_SIZE		6
#define RUIJIE_LOG_NUM_MAX	100
#define RUIJIE_LOG_MSG_LEN	256
#define RUIJIE_LOG_TIME_LEN	50

#define ruijie_SwitchCall(msg,pa,pb,pc) nc_ipc(IPC_PATH_RUIJIE, msg,3,pa,pb,pc)
typedef unsigned char uchar;
typedef u_int32_t ip_t;

struct  ruijie_log {
	struct	list_head list;	// log list node
	char	time[RUIJIE_LOG_TIME_LEN];	
	char	log[RUIJIE_LOG_MSG_LEN];
};

struct ruijie_logs {
	int 	log_num;
	struct	list_head list;	// log list head
};

/* parameters that ruijieclient and wan config related
 *
 * */
struct ruijie_param {
	int	enable;
	char	user[IGD_NAME_LEN];
	char	pass[IGD_MIN_BUFSZ];

	/* 0 : disable
	 * 1 : DHCP before auth
	 * 2 : DHCP after auth
	 * 3 : DHCP after auth and reauth with new ip
	 */
	int	dhcp_mode; 

	/* 0 : standard broadcast addr
	 * 1 : ruijie private broadcast addr
	 * */
	int	broadcast_mode;

	uchar 	mac[MAC_SIZE];
	uchar 	mac_clone[MAC_SIZE];
	uchar 	mac_default[MAC_SIZE];
	struct	in_addr	host_ip;	// use to mac clone

	/* The following parameters enabled only if dhcp_mode is 0
	 * then we have to set static ip param
	 * ip, gateway, mask and dns is network byte order
	 */
	struct 	in_addr ip;
	struct 	in_addr gateway;
	struct 	in_addr mask;
	struct 	in_addr dns[IGD_INTERFACE_DNS_NUM];
};

enum{
	DHCP_MODE_DISABLE,
	DHCP_MODE_BEFORE_AUTH,
	DHCP_MODE_AFTER_AUTH,
	DHCP_MODE_AFTER_AUTH_REAUTH
}DHCP_MODE;


enum{
	RUIJIE_PARAM_SET,
	RUIJIE_PARAM_GET,
	RUIJIE_LOG_NEXT,
	RUIJIE_LOG_ADD,
	RUIJIE_LOG_CLEAN
}RUIJIE_APP_CGI;

#endif // RUIJIE_H
