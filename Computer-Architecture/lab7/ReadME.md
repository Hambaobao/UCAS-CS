# 国科大体系机构实验（2019）lab7

实验任务：

1.  在lab6的基础上添加更多的指令，具体包括转移类指令BGEZ、BGTZ、BLEZ、BLTZ、J、BLTZAL、BGEZAL、JALR，乘除运算类指令MULT、MULTU、DIV、DIVU，以及 访存指令LB、LBU、LH、LHU、LWL、LWR、SB、SH、SWL、SWR；

## 文件目录
|
|--lab7  
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
|   |--lab7.pdf :	实验报告
|   |
|   |--ReadME	:本文档
|