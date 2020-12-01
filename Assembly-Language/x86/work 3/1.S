.section .data

iostring:

 .asciz "ab1g2hA0H56po9wK78nB"

.section .text

.globl _start

_start:

	movl	$iostring, %ebx

	movl	$0, %edi

loop_start:

	cmpl	$20, %edi

	je	loop_exit

	mov	(%ebx), %al	

	cmp	$'a', %al

	jl	out_of
	 
	cmp	$'z', %al

	jg	out_of		

	sub	$32, %al

	mov	%al, (%ebx)

	incl	%edi
	
	incl	%ebx
	
	jmp	loop_start
	
out_of:
	
	incl	%edi

	incl	%ebx	

	jmp	loop_start


loop_exit:

#output

         movl          $4, %eax

         movl          $1, %ebx

         movl          $iostring, %ecx

	 movl	       %edi, %edx	

         int    $0x80

#exit

         movl          $1, %eax

         movl          $0, %ebx

         int    $0x80
