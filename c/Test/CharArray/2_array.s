	.file	"2_array.c"
	.globl	a
	.data
	.align 32
	.type	a, @object
	.size	a, 100
a:
	.ascii	"helloworld"
	.string	"helloworl"
	.zero	80
	.section	.rodata
	.align 4
.LC0:
	.string	"a=%x\na+1=%x\na[1]+1=%x\n*a=%x\n&a[1][1]=%x\n&a[1]=%x\n"
.LC1:
	.string	"2_array.c"
.LC2:
	.string	"%s:%d:%s\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$32, %esp
	movl	$a+10, 24(%esp)
	movl	$a+11, 20(%esp)
	movl	$a, 16(%esp)
	movl	$a+11, 12(%esp)
	movl	$a+10, 8(%esp)
	movl	$a, 4(%esp)
	movl	$.LC0, (%esp)
	call	printf
	movl	$.LC1, 12(%esp)
	movl	$12, 8(%esp)
	movl	$__FUNCTION__.1816, 4(%esp)
	movl	$.LC2, (%esp)
	call	printf
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
	.type	__FUNCTION__.1816, @object
	.size	__FUNCTION__.1816, 5
__FUNCTION__.1816:
	.string	"main"
	.ident	"GCC: (Debian 4.7.2-5) 4.7.2"
	.section	.note.GNU-stack,"",@progbits
