/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * 
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * 
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE. 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * */

#ifndef INCLUDE_REGS_H_
#define INCLUDE_REGS_H_

/* zero register, its content is always 0 */
#define zero        $0
 
/* reserved for assembler */
#define AT          $1

/* reserved for function return */
#define v0          $2
#define v1          $3

/* reserved for function parameter */
#define a0          $4
#define a1          $5
#define a2          $6
#define a3          $7


/* temporary register */
#define t0          $8
#define t1          $9
#define t2          $10
#define t3          $11
#define t4          $12
#define t5          $13
#define t6          $14
#define t7          $15
#define t8          $24
#define t9          $25
 
#define ta0         $12
#define ta1         $13
#define ta2         $14
#define ta3         $15

#define s0          $16
#define s1          $17
#define s2          $18
#define s3          $19
#define s4          $20
#define s5          $21
#define s6          $22
#define s7          $23
#define s8          $30

/* operating system & exception handling retention */
/* These two registers will not be used during normal operation. */
#define k0          $26
#define k1          $27

/* global pointer */
#define gp          $28

/* stack pointer */
#define sp          $29

/* frame pointer */
#define fp          $30

/* saved return address */
#define ra          $31

/* This is for struct TrapFrame in scheduler.h
 * Stack layout for all exceptions:
 *
 * ptrace needs to have all regs on the stack. If the order here is changed,
 * it needs to be updated in include/asm-mips/ptrace.h
 *
 * The first PTRSIZE*5 bytes are argument save space for C subroutines.
 */

#define OFFSET_REG0         0
#define OFFSET_REG1         4
#define OFFSET_REG2         8
#define OFFSET_REG3         12
#define OFFSET_REG4         16
#define OFFSET_REG5         20
#define OFFSET_REG6         24
#define OFFSET_REG7         28
#define OFFSET_REG8         32
#define OFFSET_REG9         36
#define OFFSET_REG10        40
#define OFFSET_REG11        44
#define OFFSET_REG12        48
#define OFFSET_REG13        52
#define OFFSET_REG14        56
#define OFFSET_REG15        60
#define OFFSET_REG16        64
#define OFFSET_REG17        68
#define OFFSET_REG18        72
#define OFFSET_REG19        76
#define OFFSET_REG20        80
#define OFFSET_REG21        84
#define OFFSET_REG22        88
#define OFFSET_REG23        92
#define OFFSET_REG24        96
#define OFFSET_REG25        100
/* $26 (k0) and $27 (k1) not saved */
#define OFFSET_REG26        104
#define OFFSET_REG27        108
#define OFFSET_REG28        112
#define OFFSET_REG29        116
#define OFFSET_REG30        120
#define OFFSET_REG31        124

#define OFFSET_STATUS       128

#define OFFSET_HI           132
#define OFFSET_LO           136

#define OFFSET_BADVADDR     140
#define OFFSET_CAUSE        144
#define OFFSET_EPC          148
#define OFFSET_PC           152
/* Size of stack frame, word/double word alignment */
#define OFFSET_SIZE         156

/*
 * Copyright (C) 1994, 1995, 1996, 1997, 2000, 2001 by Ralf Baechle
 *
 * Copyright (C) 2001, Monta Vista Software
 * Author: jsun@mvista.com or jsun@junsun.net
 */

#define CP0_INDEX           $0
#define CP0_RANDOM          $1
#define CP0_ENTRYLO0        $2
#define CP0_ENTRYLO1        $3
#define CP0_CONTEXT         $4
#define CP0_PAGEMASK        $5
#define CP0_WIRED           $6
#define CP0_BADVADDR        $8
#define CP0_COUNT           $9
#define CP0_ENTRYHI         $10
#define CP0_COMPARE         $11
#define CP0_STATUS          $12

/* --------- CP0_STATUS (low 16bits) ------------ *
 * NAME | IM7~IM0  | 000 | KSU | ERL | EXL | IE |
 * ----------------------------------------------
 * BITS |     8    |  3  |  2  |  1  |  1  |  1 |
 * ----------------------------------------------
 * INIT | 1000 0000| 000 | 00  |  0  |  0  |  1 |
 * ---------------------------------------------- */
#define CP0_CAUSE           $13
/* ----- CP0_CAUSE (low 16bits) ------ *
 * NAME |  IP7~IP0  | 0 | ExcCode | 0 |
 * -----------------------------------
 * BITS |     8     | 1 |    5    | 2 |
 * ----------------------------------- */

#define CP0_EPC             $14
#define CP0_PRID            $15
#define CP0_CONFIG          $16
#define CP0_LLADDR          $17
#define CP0_WATCHLO         $18
#define CP0_WATCHHI         $19
#define CP0_XCONTEXT        $20
#define CP0_FRAMEMASK       $21
#define CP0_DIAGNOSTIC      $22
#define CP0_PERFORMANCE     $25
#define CP0_ECC             $26
#define CP0_CACHEERR        $27
#define CP0_TAGLO           $28
#define CP0_TAGHI           $29
#define CP0_ERROREPC        $30

#define STATUS_CU0          0x10000000
#define STATUS_IE           0x1
#define CAUSE_IPL           0xff00
#define CAUSE_EXCCODE       0x7c

#endif
