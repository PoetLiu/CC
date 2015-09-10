#ifndef _MEM_H
#define _MEM_H
#include <stdio.h>
#include "debug.h"

#define IS_NULL(p)			(p == NULL)
#define P_VALID_CHECK_ACT(p, act)	{if (IS_NULL(p)) {DEBUG(#p" is NULL\n");act;}}
#define P_VALID_CHECK_RET(p, ret)     	P_VALID_CHECK_ACT(p, return ret) 
#define SAFE_FREE(p)			{if (!IS_NULL(p)){ free(p);p = NULL;}}
#define ARRAY_SIZE_GET(a)		(sizeof(a)/sizeof(a[0]))

#endif	// _MEM_H



