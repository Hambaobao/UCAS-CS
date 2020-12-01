IntHandler:

#保存寄存器
    pushw   %ax
    pushw   %di

#建立一个新的索引  
    movw    %si,    %di
1:
    movb    (%si),  %al

#测试是否是字符串结尾
    test    %al,    %al
    jz      3f

#判断是不是数字
    cmp     $0x30,  %al
    jb      2f
    cmp     $0x39,  %al
    ja      2f

#是数字则跳过
    inc     %si
    jmp     1b    

#不是数字的保留
2:
    movb    %al,    (%di)

    inc     %si
    inc     %di
    
    jmp     1b

#结尾处添上'\0'    
3:
    movb    %al,    (%di)

#还原寄存器
    popw    %di
    popw    %ax

#退出中断
    iret
   