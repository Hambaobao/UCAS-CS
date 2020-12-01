# **国科大体系机构实验（2019）lab11**
郭豪 2017K8009929011 张磊 2017K8009922027  
箱子号 39
## 实验任务

1. CPU顶层修改为AXI接口
2. 集成到SoC_AXI_Lite系统中
3. 完成固定延迟和随机延迟的功能测试

## 文件目录

lab11_39:
│  lab11_39.pdf　　　　　　         实验报告  
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
　　　　　tools.v                  译码器等工具  
　　　　　top.v                    cpu_top，顶层文件  
　　　　　WB_stage.v               写回阶段  