#ifndef _DEBUG_H
#define _DEBUG_H
#define DEBUG(fmt, args...)	\
	do {			\
		printf("DEBUG:%s-%s-%d "fmt, __FILE__, __FUNCTION__, __LINE__, ##args);\
	}while(0)

#define ERROR(fmt, args...)	\
	do {			\
		printf("ERROR:%s-%s-%d "fmt, __FILE__, __FUNCTION__, __LINE__, ##args);\
	}while(0)
#endif	// _DEBUG_H
