	.file	"hello.c"
	.section	.rodata
.LC0:
	.string	"hello.c"
.LC1:
	.string	"%s:%s:%d:hello, world!\n"
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
	subl	$16, %esp
	movl	$5, 12(%esp)
	movl	$__FUNCTION__.1815, 8(%esp)
	movl	$.LC0, 4(%esp)
	movl	$.LC1, (%esp)
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
	.type	__FUNCTION__.1815, @object
	.size	__FUNCTION__.1815, 5
__FUNCTION__.1815:
	.string	"main"
	.ident	"GCC: (Debian 4.7.2-5) 4.7.2"
	.section	.note.GNU-stack,"",@progbits
