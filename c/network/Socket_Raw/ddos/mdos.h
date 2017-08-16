#ifndef __MDOS_H__
#define __MDOS_H__

#ifdef __DEBUG__
#define DEBUG(fmt,args...) 										\
	do {												\
		printf("DEBUG:""%s:%s:%d "fmt, __FILE__, __FUNCTION__, __LINE__, ##args);		\
	}while(0)

#define ERROR(fmt,args...)										\
	do {												\
		char buf[1024];										\
		snprintf(buf, sizeof(buf), "DEBUG:""%s:%s:%d "fmt, __FILE__, __FUNCTION__, __LINE__, ##args);	\
		perror(buf);										\
	}while(0)
#else
#define DEBUG(fmt,args...) do{}while(0)
#define ERROR(fmt,args...) do{}while(0)
#endif

struct psd_header{
	unsigned long saddr;	// 源地址
	unsigned long daddr;	// 目的地址
	char	mbz;		// 置空
	char	ptcl;		// 协议类型
	unsigned short	tcpl;	// tcp 长度
};

#endif

