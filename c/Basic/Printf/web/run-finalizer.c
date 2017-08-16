#include <stdlib.h>
#include <stdio.h>
 
#define ADDR 0x00000008049584
 
void hello()
{
	    printf("hello world\n");
}
 
int main(int argc, char *argv[])
{
	    (*((unsigned long int*)ADDR))= (unsigned long    int)hello;
	    }
