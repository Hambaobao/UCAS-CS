#WORK2
.section .data
	stringvar: .ascii   "0123456789abcdef"
.section .text
.globl _start
_start:

        movl     $stringvar, %edx
	movl	 $0, %edi 
loop_start:
	cmpl	$16, %edi
	je	loop_exit
	
	incl	stringvar(,%edi,1)
	incl	%edi
	decl	stringvar(,%edi,1)
	incl	%edi
	jmp	loop_start
	
loop_exit:

#output
         movl          $4, %eax
         movl          $1, %ebx
         movl          $stringvar, %ecx
         movl          $16, %edx
         int    $0x80
#exit
         movl          $1, %eax
         movl          $0, %ebx
         int    $0x80
