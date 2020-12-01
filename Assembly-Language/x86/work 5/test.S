.code32
.section .data
	var: .int 0x8912abcd
.section .text
.globl _start

_start:
	pushl var
	call print_hex
	mov $1, %eax
	mov $0, %ebx
	int $0x80

.type print_hex, @function
print_hex:
	push %ebp
	mov %esp, %ebp
	sub $16, %esp
	mov %ebx, 12(%esp)
	movb $'0', 0(%esp)
	movb $'x', 1(%esp)
	movb $0, 10(%esp)
	lea 9(%esp), %eax
	lea 2(%esp), %ebx
L1:
	mov %ecx, %edx
	andb $0xf,%dl
	cmpb $10, %dl
	jb L2
	addb $0x57, %dl
	jmp L3
L2:
	orb $0x30, %dl
L3:
	movb %dl, (%eax)
	dec %eax
	shrl $4, %ecx
	cmp %ebx, %eax
	jae L1
	mov $4, %eax
	mov $1,	%ebx
	mov %esp, %ecx
	mov $10, %edx
	int $0x80
	mov 12(%esp), %ebx
	leave
	ret
	
