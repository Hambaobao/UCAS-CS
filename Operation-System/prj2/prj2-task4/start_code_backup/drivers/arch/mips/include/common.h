/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
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

#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include "type.h"

#define PORT_URT0   0xbfe40000
#define PORT_URT1   0xbfe44000
#define PORT_URT2   0xbfe48000
#define PORT_URT3   0xbfec0000
#define PORT_URT4   0xbfec4000
#define PORT_URT5   0xbfec5000
#define PORT_URT6   0xbfec6000
#define PORT_URT7   0xbfec7000
#define PORT_URT8   0xbfec8000
#define PORT_URT9   0xbfec9000
#define PORT_URT10  0xbfeca000
#define PORT_URT11  0xbfecb000

#define REG_DAT     0x00
#define REG_IER     0x01
#define REG_IIR     0x02
#define REG_FCR     0x02
#define REG_LCR     0x03
#define REG_MCR     0x04
#define REG_LSR     0x05
#define REG_MSR     0x06
#define REG_CR      0x08
#define REG_MR      0x09

#define COLOR_RED      "\e[31m"
#define COLOR_GREEN    "\e[32m"
#define COLOR_YELLOW   "\e[33m"
#define COLOR_BLUE     "\e[34m"
#define COLOR_MAGENTA  "\e[35m"
#define COLOR_CYAN     "\e[36m"
#define COLOR_RESET    "\e[0m"

// enter a char into serial port
// use PMON printch function
void port_write_ch(char ch);

// enter a message into seraial port
// use PMON printstr function
void port_write(char *buf);

#endif
