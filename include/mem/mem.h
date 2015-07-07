#ifndef _MEM_H
#define _MEM_H
#include <stdio.h>
#include "debug.h"

#define IS_NULL(p)		(p == NULL)
#define TEST_P_NULL(p, ret)	{if (IS_NULL(p)) {DEBUG(#p"is NULL");return ret;}}
#define SAFE_FREE(p)		{if (!IS_NULL(p)){ free(p);p = NULL;}}
#define GET_ARRAY_SIZE(a)	(sizeof(a)/sizeof(a[0]))

#endif	// _MEM_H



