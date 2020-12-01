#main.S
.section .data
	var: .int 0x0812abcd

.section .text

.globl _start

_start:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	var
	call	print_hex

	mov	$1, %eax
	mov	$0, %ebx
	int	$0x80

.type print_hex, @function
print_hex:
	pushl	%ebp
	movl	%esp,	%ebp
	subl	$11,	%esp
	
	movl	var,	%eax
	movl	$0,	%edx

	andl	$0xf0000000,	%eax
	shrl	$28,	%eax

	cmpb	$8,	%al
	jge	L1
L0:
	movb	$'0',	(%esp,%edx,1)
	incl	%edx
	movb	$'x',	(%esp,%edx,1)
	incl	%edx
	movl	$0,	%edi

LOOP_START:
	cmpl	$8,	%edi
	jg	EXIT

	movl	var,	%eax
	andl	$0xf0000000,	%eax
	shrl	$28,	%eax

	cmpb	$0,	%al
	je	EQU

	cmpb	$10,	%al
	jge	ALP
	jl	NUM

L2:
	incl	%edi
	shll	$4,	(var)	
	jmp	LOOP_START

L1:
	movb	$'-',	(%esp,%edx,1)
	notl	(var)
	incl	(var)
	incl	%edx
	jmp	L0

ALP:
	addl	$87,	%eax
	movb	%al,	(%esp,%edx,1)
	incl	%edx
	jmp	L2

NUM:
	addl	$48,	%eax
	movb	%al,	(%esp,%edx,1)
	incl	%edx
	jmp	L2

EQU:
	shll	$4,	(var)
	incl	%edi
	jmp	LOOP_START

EXIT:
	mov	$4,	%eax
	mov	$1,	%ebx
	mov	%esp,	%ecx

	int	$0x80
	leave
	ret
			

		
