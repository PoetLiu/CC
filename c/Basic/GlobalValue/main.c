/*
 *
 * Authored by Liu Peng on: Thu Nov 13 17:58:06 CST 2014
 *
 * @desc: 测试函数库中的全局变量在以下情况下不同进程调用时的值
 *	  1. 动态链接
 *	  2. 静态链接
 * @func:
**/
#include <stdio.h>

int main(int ac, char *av[])
{
	int	k;

	k	= av[1][0] - '0';
	func(k);
	sleep(10);
	return 0;
}
