.section .data 
output: 
  .ascii "hello,syscall!!!!\n" 
output_end: 
  .equ len,output_end - output 
.section .text 
.globl _start 
_start: 
  mov $4,%al #define __NR_write 4
  mov $1,%bl 
  mov $output,%ecx 
  mov $len,%dl 
  int $0x80 
  mov $1,%al
  mov $0,%bl 
  int $0x80 
