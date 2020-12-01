.section .data

	iostring:	.asciz "ab1g2hA0H56po9wK78nB"

.section .text

.globl __start

__start:
#	.set	noreorder
#	.set	nomacro

	la	$t0, iostring

# t5用于记录已经进行了操作的字符个数，字符串总共20个字符
	addiu	$t5, $0, 1

L1:
#如果t5大于20则跳转到输出
	sltiu	$t4, $t5, 20
	blez	$t4, OUTPUT
#将iosstring里的base处的字符加载到t1
	lb	$t1, 0($t0)
#如果t1的内容小于z，则跳转到LTZ
	sltiu	$t4, $t1, 'z'
	bgtz	$t4, LTZ
	b	OUT

LTZ:
#如果t1的内容小于a，则跳转到OUT
	sltiu	$t4, $t1, 0x60
	bgtz	$t4, OUT
#将小写字母转化为大写字母
	addi	$t1, $t1, -32
#存回iostring
	sb	$t1, 0($t0)
#iostring的base加8,到下一个字符
	addiu	$t0, $t0, 1
#已经操作的字符数+1
	addiu	$t5, $t5, 1
#跳转到L1
	b	L1


#到这里，说明$t1的字符不是小写字母
OUT:
#iostring的base加8,到下一个字符
	addiu	$t0, $t0, 1
#已经操作的字符数+1
	addiu	$t5, $t5, 1
	b	L1
	
#output
OUTPUT:
	addiu	$v0, $0, 4004
	addiu	$a0, $0, 1

	la	$a1, iostring

	addiu	$a2, $0, 20
	syscall
#exit
	addiu	$v0, $0, 4001
	addiu	$a0, $0, 0
	syscall



