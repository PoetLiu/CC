# call system func 'write' print 'hello' to console
# COMPILE
# as -o syscall.o syscall.s
# then
# ld -o syscall syscall.o

.section .text 
.globl _start 
_start: 
  xorl %eax,%eax 
  pushl %eax 
  pushl $0x68732f6e 
  pushl $0x69622f2f 
  movl %esp,%ebx 
  pushl %eax 
  pushl %ebx 
  movl %esp,%ecx 
  movb $0xb,%al 
  int $0x80
