#include "asm.h"

LEAF(invoke_syscall)
    // TODO syscall entry

    add     v0, a0, zero
    add     a0, a1, zero
    add     a1, a2, zero
    add     a2, a3, zero
    syscall
    jr      ra

END(invoke_syscall)