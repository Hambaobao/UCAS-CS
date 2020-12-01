.section .data
	output: .ascii "hello world\n"

.section .text
	.globl __start

__start:
	addiu	$v0, $0, 4004
	addiu	$a0, $0, 1
	la	$a1, output
	addiu	$a2, $0, 12
	syscall
	addiu	$v0, $0, 4001
	addiu	$a0, $0, 0
	syscall
