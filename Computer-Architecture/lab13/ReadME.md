# **国科大体系机构实验（2019）lab13**
郭豪 2017K8009929011 张磊 2017K8009922027  
箱子号 39
## 实验任务

1. CPU 增加 TLBR、TLBWI、TLBP 指令； 
2. CPU增加 index、entryhi、entrylo0、entrylo1寄存器； 
3. CPU增加 16项 TLB结构支持，支持页的大小为 4KB； 
4. 运行专用功能测试 tlb_func，要求通过前 6项测试

## 文件目录

lab13_39:
│  lab13_39.pdf　　　　　　         实验报告  
│  ReadME.md  
│  
└─myCPU  
　　　　　alu.v 　　　　　　　　　   ALU模块   
　　　　　cp0.v 　　　　　　　　　   CP0模块  
　　　　　cpu_axi_interface.v　　　 转接桥模块   
　　　　　EXE_stage.v  　　　　　　 执行阶段  
　　　　　ID_stage.v  　　　　　　　译码阶段  
　　　　　IF_stage.v  　　　　　　　取指阶段  
　　　　　MEM_stage.v              访存阶段  
　　　　　mycpu.h                  头文件  
　　　　　mycpu_core.v             CPU核顶层文件   
　　　　　regfile.v                寄存器堆
         tlv.v                    TLB模块
　　　　　tools.v                  译码器等工具  
　　　　　top.v                    cpu_top，顶层文件  
　　　　　WB_stage.v               写回阶段  