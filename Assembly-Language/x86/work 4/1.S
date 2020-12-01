.section .data

 testdata:

   .int 6, -5, 3, 1, -8, 7, 0, 2, 4, 9

.section .text

.globl _start

_start:

	movl $testdata, %ebx	

	movl $1, %edx
	
	call insert_sort

#output

	call as_puts

#exit

	call as_exit

	.type insert_sort, @function
insert_sort:

out_loop:

	cmpl	$11, %edx
	
	je	as_puts
	
	movl	%edx, %edi
	
	decl	%edi
	
	movl	(%ebx,%edx,4),	%ecx
	
	incl	%edx
	
	jmp	inner_loop

	ret
	
	
inner_loop:

	cmpl	$-1, %edi

	je	out
	
	cmpl	(%ebx,%edi,4),	%ecx
	
	jl	move
	
	movl	%ecx, 4(%ebx,%edi,4)

	jmp	out_loop
	
move:
	
	movl	(%ebx,%edi,4),	%eax

	movl	%eax,	4(%ebx,%edi,4)
	
	decl	%edi
	
	jmp	inner_loop

out:

	movl	%ecx, 4(%ebx,%edi,4)

	jmp	out_loop


	.type	as_puts, @function
as_puts:

	movl	$4, %eax
	
	movl	$1, %ebx
	
	movl	$testdata, %ecx
	
	movl	$10, %edx
	
	int	$0x80

	ret
	
	.type	as_exit, @function
as_exit:

	movl	$1, %eax
	
	movl	$0, %ebx
	
	int	$0x80	
	
	
	
	
	
	

	

	
