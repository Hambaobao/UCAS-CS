.section .data
	bytevar:
		.rept 10
		.byte 0x0
		.endr
.section .text

.globl __start
__start:
	.set	noreorder
	.set	nomacro

	lui	$t0, %hi(bytevar)
	addiu	$t0, $t0, %lo(bytevar)
	addu	$a1, $0, $t0
	addiu	$t1, $0, 0x30

L1:
	sb	$t1, 0($t0)
	addiu	$t1, $t1, 1
	sltiu	$t3, $t1, 0x3a

	bgtz	$t3, L1
	addiu	$t0, $t0, 1

#output
	addiu	$v0, $0, 4004
	addiu	$a0, $0, 1
#	la	$a1, bytevar
	addiu	$a2, $0, 10
	syscall
#exit
	addiu	$v0, $0, 4001
	addiu	$a0, $0, 0
	syscall

 
