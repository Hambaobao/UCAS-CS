/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *                Interrupt processing related implementation
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
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
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * */
#ifndef INCLUDE_INTERRUPT_H_
#define INCLUDE_INTERRUPT_H_

#include "type.h"

/* ERROR code */
enum ExcCode
{
    /* 14, 16-22, 24-31 is reserver ExcCode */
    INT,       // 0
    MOD,       // 1
    TLBL,      // 2
    TLBS,      // 3
    ADEL,      // 4
    ADES,      // 5
    IBE,       // 6
    DBE,       // 7
    SYS,       // 8
    BP,        // 9
    RI,        // 10
    CPU,       // 11
    OV,        // 12
    TR,        // 13
    FPE = 15,  // 15
    WATCH = 23 // 23
};

#define ExcCode 0x7c

/* BEV = 0 */
// Exception Enter Vector
#define BEV0_EBASE 0x80000000
#define BEV0_OFFSET 0x180

/* BEV = 1 */
#define BEV1_EBASE 0xbfc00000
#define BEV1_OFFSET 0x380

#define TIMER_INTERVAL 150000

void interrupt_helper(uint32_t, uint32_t);

/* exception handler entery */
extern void exception_handler_entry(void);
extern void exception_handler_begin(void);
extern void exception_handler_end(void);

extern void handle_int(void);
extern void handle_syscall(void);
extern void handle_other(void);

#endif