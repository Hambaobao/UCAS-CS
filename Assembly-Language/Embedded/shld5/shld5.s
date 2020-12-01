	.file	"shld5.c"
	.text
	.globl	shld5
	.type	shld5, @function
shld5:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	.cfi_offset 3, -24
	movl	%edi, -28(%rbp)
	movl	%esi, -32(%rbp)
	movl	-32(%rbp), %edx
	movl	-28(%rbp), %eax
	movl	%edx, %ebx
#APP
# 9 "shld5.c" 1
	movl	%eax, %eax
	movl	%ebx, %ebx
	shll	%eax, 5
	shrl	%ebx, 27
	orl	%ebx, %eax
	
# 0 "" 2
#NO_APP
	movl	%eax, -12(%rbp)
	movl	-12(%rbp), %eax
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	shld5, .-shld5
	.ident	"GCC: (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
