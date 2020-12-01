# 国科大体系机构实验（2019）lab8

在lab7的基础上增加了CP0寄存器模块，添加了MFC0，MTC0，SYSCALL，ERET指令，即增加了SYSCALL支持；

## 文件目录
|
|--lab8_39  
|   |--rtl/	：实验源码
|   |   |-- BRIDGE/    
|   |   |
|   |   |-- CONFREG/ 
|   |   |
|   |   |-- xilinx_ip/
|   |   |
|   |   |-- myCPU/: 
|   |   |   |--cp0.v	          : CP0寄存器模块代码 
|   |   |   |
|   |   |   |-- MEM_stage.v    : 流水线访存阶段代码
|   |   |   | 
|   |   |   |-- EXE_stage.v       : 流水线执行阶段代码
|   |   |   | 
|   |   |   |-- ID_stage.v         : 流水线译码阶段代码
|   |   |   | 
|   |   |   |-- WB_stage.v       : 流水线写回阶段代码 
|   |   |   | 
|   |   |   |-- IF_stage.v          : 流水线取指阶段代码
|   |   |   |
|   |   |   |-- tools.v               : 译码器等工具 
|   |   |   | 
|   |   |   |-- alu.v                  : alu代码
|   |   |   | 
|   |   |   |-- regfile.v             : 寄存器堆代码
|   |   |   | 
|   |   |   |-- my_cpu_top.v    : cpu顶层代码 
|   |   |   | 
|   |   |   |-- mycpu.h            : cpu头文件
|   |   |   |  
|   |   |--soc_lite_top.v
|   |
|   |--lab8_39.pdf :	实验报告
|   |
|   |--ReadME	:本文档
|