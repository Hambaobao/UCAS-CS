Inthandler:

	pushw	%ax

1:	movb	(%si),	%al

	test	%al,	%al

	jz		1f

	subb	$'a',	%al

	cmp		$26,	%al

	jae		2f

	addb	$0x41,	%al

	movb	%al,	(%si)

2:	inc		%si

	jmp		1b

1:	popw	%ax

	iret
	
