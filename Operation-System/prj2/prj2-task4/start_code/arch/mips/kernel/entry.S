#include "asm.h"

.globl  init_handle
.equ    KERNEL, 0
.equ    USER,   156

.macro STI
    mfc0    $26, CP0_STATUS
    ori     $26, $26, 0x1
    mtc0    $26, CP0_STATUS
.endm

.macro CLI
    mfc0    $26, CP0_STATUS
    li	    $27, 0xfffffffe		
    and     $26, $26, $27
    mtc0    $26, CP0_STATUS
.endm

LEAF(asm_start)
    mtc0    zero, CP0_STATUS
    mtc0    zero, CP0_WATCHLO
    mtc0    zero, CP0_WATCHHI

    mfc0    t0, CP0_CONFIG
    and     t0, ~0x7
    ori     t0, 0x2
    mtc0    t0, CP0_CONFIG
    
    jr  ra
END(asm_start)

LEAF(GET_CP0_STATUS)
	mfc0	v0, CP0_STATUS
	jr		ra
END(GET_CP0_STATUS)

LEAF(SET_CP0_STATUS)
	mtc0	a0, CP0_STATUS
	jr		ra
END(SET_CP0_STATUS)

LEAF(GET_CP0_COUNT)
	mfc0	v0, CP0_COUNT
	jr		ra
END(GET_CP0_COUNT)

LEAF(SET_CP0_COUNT)
	mtc0	a0, CP0_COUNT
	jr		ra
END(SET_CP0_COUNT)

LEAF(GET_CP0_COMPARE)
	mfc0	v0, CP0_COMPARE
	jr		ra
END(GET_CP0_COMPARE)

LEAF(SET_CP0_COMPARE)
	mtc0	a0, CP0_COMPARE
	jr		ra
END(SET_CP0_COMPARE)

LEAF(GET_CP0_EPC)
	mfc0	v0, CP0_EPC
	jr		ra
END(GET_CP0_EPC)

LEAF(SET_CP0_EPC)
	mtc0	a0, CP0_EPC
	jr		ra
END(SET_CP0_EPC)

.macro SAVE_CONTEXT offset
    // TODO save context

    lw		$26, current_running		#
    addiu   $26, $26, \offset 
   
    sw		$0, OFFSET_REG0($26)		# 
    sw      $1, OFFSET_REG1($26)
    sw      $2, OFFSET_REG2($26)
    sw      $3, OFFSET_REG3($26)
    sw      $4, OFFSET_REG4($26)
    sw      $5, OFFSET_REG5($26)
    sw      $6, OFFSET_REG6($26)
    sw      $7, OFFSET_REG7($26)
    sw		$8, OFFSET_REG8($26)		# 
    sw      $9, OFFSET_REG9($26)
    sw      $10, OFFSET_REG10($26)
    sw      $11, OFFSET_REG11($26)
    sw      $12, OFFSET_REG12($26)
    sw      $13, OFFSET_REG13($26)
    sw      $14, OFFSET_REG14($26)
    sw		$15, OFFSET_REG15($26)		# 
    sw      $16, OFFSET_REG16($26)
    sw      $17, OFFSET_REG17($26)
    sw      $18, OFFSET_REG18($26)
    sw      $19, OFFSET_REG19($26)
    sw      $20, OFFSET_REG20($26)
    sw      $21, OFFSET_REG21($26)
    sw		$22, OFFSET_REG22($26)		# 
    sw      $23, OFFSET_REG23($26)
    sw      $24, OFFSET_REG24($26)
    sw      $25, OFFSET_REG25($26)

    sw      $28, OFFSET_REG28($26)
    sw      $29, OFFSET_REG29($26)
    sw      $30, OFFSET_REG30($26)
    sw      $31, OFFSET_REG31($26)

    mfc0    $27, CP0_STATUS
    sw      $27, OFFSET_STATUS($26)
    mfc0    $27, CP0_WATCHHI
    sw      $27, OFFSET_HI($26)
    mfc0    $27, CP0_WATCHLO
    sw      $27, OFFSET_LO($26)
    mfc0    $27, CP0_BADVADDR
    sw      $27, OFFSET_BADVADDR($26)
    mfc0    $27, CP0_CAUSE
    sw      $27, OFFSET_CAUSE($26)
    mfc0    $27, CP0_EPC
    sw      $27, OFFSET_EPC($26)
#    sw      $31, OFFSET_PC($26)

.endm

.macro RESTORE_CONTEXT offset
    // TODO restore context

    lw      $26, current_running
    addiu   $26, $26, \offset

    lw		$0, OFFSET_REG0($26)		# 
    lw      $1, OFFSET_REG1($26)
    lw      $2, OFFSET_REG2($26)
    lw      $3, OFFSET_REG3($26)
    lw      $4, OFFSET_REG4($26)
    lw      $5, OFFSET_REG5($26)
    lw      $6, OFFSET_REG6($26)
    lw      $7, OFFSET_REG7($26)
    lw		$8, OFFSET_REG8($26)		# 
    lw      $9, OFFSET_REG9($26)
    lw      $10, OFFSET_REG10($26)
    lw      $11, OFFSET_REG11($26)
    lw      $12, OFFSET_REG12($26)
    lw      $13, OFFSET_REG13($26)
    lw      $14, OFFSET_REG14($26)
    lw		$15, OFFSET_REG15($26)		# 
    lw      $16, OFFSET_REG16($26)
    lw      $17, OFFSET_REG17($26)
    lw      $18, OFFSET_REG18($26)
    lw      $19, OFFSET_REG19($26)
    lw      $20, OFFSET_REG20($26)
    lw      $21, OFFSET_REG21($26)
    lw		$22, OFFSET_REG22($26)		# 
    lw      $23, OFFSET_REG23($26)
    lw      $24, OFFSET_REG24($26)
    lw      $25, OFFSET_REG25($26)

    lw      $28, OFFSET_REG28($26)
    lw      $29, OFFSET_REG29($26)
    lw      $30, OFFSET_REG30($26)
    lw      $31, OFFSET_REG31($26)

    lw      $27, OFFSET_STATUS($26)
    mtc0    $27, CP0_STATUS
    lw      $27, OFFSET_HI($26)
    mtc0    $27, CP0_WATCHHI
    lw      $27, OFFSET_LO($26)
    mtc0    $27, CP0_WATCHLO
    lw      $27, OFFSET_BADVADDR($26)
    mtc0    $27, CP0_BADVADDR
    lw      $27, OFFSET_CAUSE($26)
    mtc0    $27, CP0_CAUSE
    lw      $27, OFFSET_EPC($26)
    mtc0    $27, CP0_EPC
#    lw      $27, OFFSET_EPC($26)
#    lw      $31, OFFSET_PC($26)
.endm

.set noat

NESTED(do_scheduler, 0, ra)
    SAVE_CONTEXT(KERNEL)
    jal     scheduler
    RESTORE_CONTEXT(KERNEL)
    jr      ra
END(do_scheduler)

.global exception_handler_begin
.global exception_handler_end

NESTED(exception_handler_entry, 0, sp)   
exception_handler_begin:
    // TODO close interrupt    
    CLI
    SAVE_CONTEXT(USER)
    
    // jmp exception_handler[i] which decided by CP0_CAUSE
    // Leve2 exception Handler. 

    mfc0    $27, CP0_CAUSE
    andi    $27, $27, CAUSE_EXCCODE

    la		$26, exception_handler		     
    add		$26, $26, $27		# $26 = $26 + $27
    lw		$26, 0($26)		 
    jr      $26

exception_handler_end:
END(exception_handler_entry)

NESTED(handle_int, 0, sp)
    // interrupt handler
    // Leve3 exception Handler.

    mfc0    a0, CP0_STATUS
    mfc0    a1, CP0_CAUSE
    
    addi    sp, sp, -8
    jal		interrupt_helper
    addi    sp, sp, 8
init_handle:
    //reset count & compare reg
	mtc0	zero, CP0_COUNT
	li		k0, 120000
	mtc0	k0, CP0_COMPARE

    // open interrupt
    RESTORE_CONTEXT(USER)    
    STI
    eret

END(handle_int)

NESTED(handle_syscall, 0, sp)
    // system call handler
    
	add		a3, a2, zero
	add		a2, a1, zero
	add		a1, a0, zero
	add		a0, v0, zero

    addi    sp, sp, -16
    jal     system_call_helper
    addi    sp, sp, 16

    RESTORE_CONTEXT(USER)
    STI
    eret
END(handle_syscall)

NESTED(handle_other, 0, sp)
    // other exception handler

    RESTORE_CONTEXT(USER)
    STI
    eret
END(handle_other)
