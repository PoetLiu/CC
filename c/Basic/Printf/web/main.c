#include <stdio.h>
 
int main(int argc, char *argv[])
{

 printf("%*c%hn%*c%hn"
  "\xeb\x3d\x48\x54\x54\x50\x2f\x31\x2e\x30\x20\x32"
 "\x30\x30\x0d\x0a\x43\x6f\x6e\x74\x65\x6e\x74\x2d"
  "\x74\x79\x70\x65\x3a\x74\x65\x78\x74\x2f\x68\x74"
 "\x6d\x6c\x0d\x0a\x0d\x0a\x3c\x68\x31\x3e\x48\x65"
  "\x6c\x6c\x6f\x20\x57\x6f\x72\x6c\x64\x21\x3c\x2f"
  "\x68\x31\x3e\x4c\x8d\x2d\xbc\xff\xff\xff\x48\x89"
 "\xe3\x48\x83\xeb\x10\x48\x31\xc0\x50\x66\xb8\x1f"
  "\x90\xc1\xe0\x10\xb0\x02\x50\x31\xd2\x31\xf6\xff"
 "\xc6\x89\xf7\xff\xc7\x31\xc0\xb0\x29\x0f\x05\x49"
 "\x89\xc2\x31\xd2\xb2\x10\x48\x89\xde\x89\xc7\x31"
  "\xc0\xb0\x31\x0f\x05\x31\xc0\xb0\x05\x89\xc6\x4c"
 "\x89\xd0\x89\xc7\x31\xc0\xb0\x32\x0f\x05\x31\xd2"
  "\x31\xf6\x4c\x89\xd0\x89\xc7\x31\xc0\xb0\x2b\x0f"
 "\x05\x49\x89\xc4\x48\x31\xd2\xb2\x3d\x4c\x89\xee"
 "\x4c\x89\xe7\x31\xc0\xff\xc0\x0f\x05\x31\xf6\xff"
 "\xc6\xff\xc6\x4c\x89\xe7\x31\xc0\xb0\x30\x0f\x05"
  "\x4c\x89\xe7\x31\xc0\xb0\x03\x0f\x05\xeb\xc3",
 ((((unsigned long int)0x8048500 + 12) >> 16) & 0xffff), 
 0, 0x08049658 + 2, 
  (((unsigned long int)0x8048500 + 12) & 0xffff)-
  ((((unsigned long int)0x8048500 + 12) >> 16) & 0xffff), 
  0, 0x08049658 );

}
