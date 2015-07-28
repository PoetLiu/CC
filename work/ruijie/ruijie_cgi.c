#include <stdio.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "ruijie.h"
#include "cgi.h"

#define CGI_360_RETURN_CODE( err_no, desc) do{\
		char err[256];\
		sprintf(err, "{\"err_no\":\"%d\",\"err_des\":\"%s\"}", err_no, desc);\
		cgi_write(html_mime_type_text);\
		cgi_write(err);\
		if(DEBUG_INFO){console_printf("out:%s, %s:line number %d \n", err, __FUNCTION__, __LINE__);}\
	    return (FP_OK|FP_OKHD|FP_FILE);\
    }while(0)
    
#define CGI_360_RETURN_DATA( data) do{\
		char _ret_buf[2048];\
		sprintf(_ret_buf, "{\"err_no\":\"0\",\"err_des\":\"success\",\"data\":[{%s}]}", data);\
		cgi_write(html_mime_type_text);\
		cgi_write(_ret_buf);\
		if(DEBUG_INFO){console_printf("out:%s, %s:line number %d \n", _ret_buf, __FUNCTION__, __LINE__);}\
		return (FP_OK|FP_OKHD|FP_FILE);\
	}while(0)

#define CGI_360_RETURN_ARRAY( data) do{\
		char _ret_buf[2048];\
		sprintf(_ret_buf, "{\"err_no\":\"0\",\"err_des\":\"success\",\"data\":[%s]}", data);\
		cgi_write(html_mime_type_text);\
		cgi_write(_ret_buf);\
		if(DEBUG_INFO){console_printf("out:%s, %s:line number %d \n", _ret_buf, __FUNCTION__, __LINE__);}\
		return (FP_OK|FP_OKHD|FP_FILE);\
	}while(0)

#define RUIJIE_DEBUG(fmt, args...) do{if(1)console_printf("\n\nDEBUG:%s:%d "fmt, __FUNCTION__, __LINE__, ##args);}while(0)
#define RUIJIE_ERROR(fmt, args...) do{if(1)console_printf("\n\nERROR:%s:%d "fmt, __FUNCTION__, __LINE__, ##args);}while(0)
int ruijie_param_set_cgi( struct httpd * hp, struct httpform * form, char ** filetext)
{
	scall_param pa = {0}, pb = {0}, pc = {0};
	int ret = 0;
    	char *p_enable = NULL, *p_dhcp_m = NULL, *p_broadcast = NULL, *p_user = NULL, *p_pass = NULL, *p_mac = NULL;
    	struct ruijie_param ruijie;
    	int enable;
	char decrypt_key[1024]= {0};

    	p_enable	= get_form_value(form, "enable");
    	p_user		= get_form_value(form, "user");
    	p_pass		= get_form_value(form, "pass");
	p_dhcp_m	= get_form_value(form, "dhcp_mode");
	p_broadcast	= get_form_value(form, "broadcast");
	p_mac		= get_form_value(form, "mac");
    	if (!p_enable || !p_user || !p_pass || !p_dhcp_m || !p_broadcast || !p_mac)
    		CGI_REQUEST_ERROR_RETURN(C_ERR_NULL);
	
	if (aes_decrypt_360(p_pass, decrypt_key, sizeof(decrypt_key)) < 0)
    		CGI_REQUEST_ERROR_RETURN(C_DATAERR);

    	memset(&ruijie, 0, sizeof(ruijie));
    	ret	= ruijie_SwitchCall(RUIJIE_PARAM_GET, PRM_POINTER(pa, &ruijie, sizeof(ruijie)), PRM_VALUE_NULL(pb), PRM_VALUE_NULL(pc));
    	if(ret != NRET_TRUE){
    	    CGI_REQUEST_ERROR_RETURN(C_DATAERR);
    	}

    	ruijie.enable		= atoi(p_enable) ? 1 : 0;
	ruijie.dhcp_mode	= atoi(p_dhcp_m); 
	ruijie.broadcast_mode	= atoi(p_broadcast); 
	CGI_STRNCPY(ruijie.user, p_user);
	CGI_STRNCPY(ruijie.pass, decrypt_key);
	str2mac(p_mac, ruijie.mac);
	
	if (!ruijie.dhcp_mode) {
		int subnet;
		char *p_ip = NULL, *p_mask = NULL, *p_gateway = NULL, *dns1 = NULL, *dns2 = NULL;
		p_ip		= get_form_value(form, "ip");
		p_mask		= get_form_value(form, "mask");
		p_gateway	= get_form_value(form, "gateway");
		dns1		= get_form_value(form, "dns1");
		dns2		= get_form_value(form, "dns2");
		
		if (!p_ip || !p_mask || !p_gateway || !dns1)
    			CGI_REQUEST_ERROR_RETURN(C_ERR_NULL);

		parse_ipad(&ruijie.ip.s_addr, &subnet, p_ip);
		parse_ipad(&ruijie.mask.s_addr, &subnet, p_mask);
		parse_ipad(&ruijie.gateway.s_addr, &subnet, p_gateway);
		parse_ipad(&ruijie.dns[0].s_addr, &subnet, dns1);
		if (dns2 && strlen(dns2))	// dns2 maybe empty 
			parse_ipad(&ruijie.dns[1].s_addr, &subnet, dns2);
	}

    	ret = ruijie_SwitchCall(RUIJIE_PARAM_SET, PRM_POINTER(pa, &ruijie, sizeof(ruijie)), PRM_VALUE_NULL(pb), PRM_VALUE_NULL(pc));
    	if(ret != NRET_TRUE){
    	    CGI_REQUEST_ERROR_RETURN(C_DATAERR);
    	}

    	CGI_REQUEST_SUCCESS_RETURN();
    	return 0;
}

int ruijie_param_get_cgi( struct httpd * hp, struct httpform * form, char ** filetext)
{
	scall_param pa={0},pb={0},pc={0};
	char *ptr, *host_ip;
	int ret = 0, i = 0;
	uchar empty_dns[MAC_SIZE] = {0};
	struct ruijie_param ruijie;
	char encrypt_key[1024]={0};

    	memset(&ruijie, 0, sizeof(ruijie));
	if (host_ip = get_cgi_env("IGD_CONNECTIP")) {
		struct in_addr hostip;
		RUIJIE_DEBUG("IGD_CONNECTIP = %s\n", host_ip);
		inet_aton(host_ip, &hostip);
		ruijie.host_ip.s_addr	= hostip.s_addr;
	} else {
		RUIJIE_DEBUG("get IGD_CONNECTIP faild\n");
	}

    	ret = ruijie_SwitchCall(RUIJIE_PARAM_GET, PRM_POINTER(pa, &ruijie, sizeof(ruijie)),PRM_VALUE_NULL(pb), PRM_VALUE_NULL(pc));
    	if(ret != NRET_TRUE){
    	    CGI_REQUEST_ERROR_RETURN(C_DATAERR);
    	}
	aes_encrypt_360_one_key(ruijie.pass, encrypt_key, sizeof(encrypt_key));
	if (!strlen(encrypt_key))
		strcpy(encrypt_key, ruijie.pass);

    	json_object_start(ptr);
    	json_value(ptr, "enable", "%d", ruijie.enable);
	json_value(ptr, "dhcp_mode", "%d", ruijie.dhcp_mode);
	json_value(ptr, "broadcast_mode", "%d", ruijie.broadcast_mode);
	json_value(ptr, "user", "%s", ruijie.user);
	json_value(ptr, "pass", "%s", ruijie.pass);
	json_value(ptr, "mac", "%s", mac2str(ruijie.mac));
	json_value(ptr, "mac_default", "%s", mac2str(ruijie.mac_default));
	json_value(ptr, "mac_clone", "%s", mac2str(ruijie.mac_clone));


	// if we use static ip
	if (!ruijie.dhcp_mode) {
		json_value(ptr, "ip", "%s", print_ipad(ruijie.ip.s_addr));
		json_value(ptr, "mask", "%s", print_ipad(ruijie.mask.s_addr));
		json_value(ptr, "gateway", "%s", print_ipad(ruijie.gateway.s_addr));
	
		json_name(ptr, "dns");
		json_array_start(ptr);
		for (i = 0; i < IGD_INTERFACE_DNS_NUM; i++)
			json_string(ptr, print_ipad(ruijie.dns[i].s_addr));
		json_eat_tail(ptr);
		json_array_end(ptr);

	}

    	json_object_end(ptr);
    	json_eat_tail(ptr);
    	CGI_GETDATA_RETURN_BUF();
}

int ruijie_log_get_cgi( struct httpd * hp, struct httpform * form, char ** filetext) 
{
	scall_param pa={0},pb={0},pc={0};
	int ret = 0, i = 0;
	int index;
	char *ptr;
	struct ruijie_log log;

 	CGI_SEND_DATA(html_mime_type_text);
        CGI_SEND_DATA(ARRAY_START);

    	memset(&log, 0, sizeof(struct ruijie_log));
	index = 0;
	while(ruijie_SwitchCall(RUIJIE_LOG_NEXT, PRM_VALUE(pa,index), PRM_POINTER(pb, &log, sizeof(struct ruijie_log)), PRM_VALUE_NULL(pc)) == NRET_TRUE){
		json_object_begin_start(ptr, index);
		json_value(ptr, "time", "%s", log.time);
		json_value(ptr, "msg", "%s", log.log);
		json_object_begin_end(ptr);
		CGI_SEND_DATA_BUF();
		index ++;
	}
	CGI_SEND_DATA(ARRAY_END);
    	CGI_GETDATA_RETURN();
}

int ruijie_log_clean_cgi( struct httpd * hp, struct httpform * form, char ** filetext) 
{

	scall_param pa={0},pb={0},pc={0};
	int ret = 0;

	ret = ruijie_SwitchCall(RUIJIE_LOG_CLEAN, PRM_VALUE_NULL(pa), PRM_VALUE_NULL(pb), PRM_VALUE_NULL(pc));
	if (ret != NRET_TRUE)
    		CGI_REQUEST_ERROR_RETURN(C_DATAERR);

    	CGI_REQUEST_SUCCESS_RETURN();
}

struct IGD_CGIMAP_ENTRY IGD_CGI_FUN_MAP[] = {
	 {"ruijie_param_set.cgi", ruijie_param_set_cgi, PERM_ALL},
	 {"ruijie_param_get.cgi", ruijie_param_get_cgi, PERM_ALL},
	 {"ruijie_log_get.cgi", ruijie_log_get_cgi, PERM_ALL},
	 {"ruijie_log_clean.cgi", ruijie_log_clean_cgi, PERM_ALL},
	 {NULL},
};

