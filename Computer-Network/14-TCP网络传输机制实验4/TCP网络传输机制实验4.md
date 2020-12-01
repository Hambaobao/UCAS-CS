---
typora-root-url: ./images
---

# TCP网络传输机制实验四报告

**张磊 2017K8009922027**



### 一、实验题目

TCP 网络传输机制实验四





### 二、 实验内容

#### 实验内容

1. 执行create_randfile.sh，生成待传输数据文件 `client-input.dat`

2. 运行给定网络拓扑（tcp_topo_loss.py）

3. 在节点 h1 上执行 TCP 程序

	  在节点 h1 上运行 TCP 协议栈的服务器模式 `./tcp_stack server 10001`

4. 在节点 h2 上执行 TCP 程序

	  在节点 h2 上运行 TCP 协议栈的客户端模式 `./tcp_stack client 10.0.0.1 10001`

	  client 向 server 发送数据，server 将数据 echo 给 client

5. 使用 md5sum 比较两个文件是否完全相同

6. 记录 h2 中每次 cwnd 调整的时间和相应值，呈现到二维坐标图中





### 三、实验流程

1. 增加实现 TCP 拥塞控制；

2. 在 tcp_in.c 中新增：当收到数据包后，根据 TCP 所处的拥塞控制状态，修改 cwnd 的数值，控制发送速率的功能
3. 记录每次修改 cwnd 的时间和数值
4. 在节点 h1 上运行 `./tcp_stack server 10001` 启动 TCP 协议栈服务器模式
5. 在节点 h2 上运行  `./tcp_stack client 10.0.0.1 10001` 启动 TCP 协议栈客户端模式
6. client 端向 server 端发送数据，server 端收到数据后存储到 server-output.dat
7. 每次实验server端完成数据接收后，执行 `md5sum server-output.dat` 和 `md5sum client-input.dat` 比较两个文件是否完全相同
8. 使用 MATLAB 将记录的 h2 的 cwnd 的调整时间和数值，呈现在二维坐标图中





### 四、实验结果

<img src="/image.jpg" alt="image" style="zoom:80%;" />

<p align="center"> result </p>



### 五、实验分析

观察实验结果，与课件给出的效果图有相同的变化趋势，慢启动状态 cwnd 数值从 1000 开始指数增长，当发声丢包后，启动快速重传和快速恢复，cwnd 数值减半， ssthress 数值页减半，当发生超时时，重新进入慢启动阶段 cwnd 又从 1000 开始指数增长，达到 ssthress 后，进入拥塞避免阶段，cwnd 数值线性增长。 





### 六、反思总结

本次实验较为简单，只需要在上次实验的基础上，添加一个用于处理 TCP 拥塞状态的状态机，在收到 ACK 后，根据所处的状态进行相应的处理，并记录下 cwnd 的数值和调整时间即可。





### 七、参考文献

----

+ [1]  中国科学院大学 2020 春计算机网络研讨课 14-网络传输机制实验四实验课件




