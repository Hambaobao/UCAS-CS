---
typora-root-url: ./images
---

# TCP网络传输机制实验二报告

**张磊 2017K8009922027**



### 一、实验题目

TCP 网络传输机制实验二





### 二、 实验内容

#### 实验内容一

1. 运行给定网络拓扑（tcp_topo.py）

2. 在节点 h1 上执行 TCP 程序

	在节点 h1 上运行 TCP 协议栈的服务器模式 `./tcp_stack server 10001`

3. 在节点 h2 上执行 TCP 程序

	在节点 h2 上运行 TCP 协议栈的客户端模式 `./tcp_stack client 10.0.0.1 10001`

	client 向 server 发送数据，server 将数据 echo 给 client

4. 使用 tcp_stack.py 替换其中任意一端，对端都能够正确收发数据



#### 实验内容二

1. 修改 tcp_apps.c（以及tcp_stack.py），使之能够收发文件

2. 执行 create_randfile.sh，生成待传输数据文件 client-input.dat

3. 运行给定网络拓扑（tcp_topo.py）

4. 在节点 h1 执行 TCP 程序

	在节点 h1 上运行 TCP 协议栈的服务器模式 `./tcp_stack server 10001`

5. 在节点 h2 上执行 TCP 程序

	在节点 h2 上运行 TCP 协议栈的客户端模式 `./tcp_stack client 10.0.0.1 10001`

	client 向 serve r发送数据，server 将收到的数据存储到文件 server-output.dat 

6. 使用 tcp_stack.py 替换其中任意一端，对端都能够正确收发数据





### 三、实验流程

#### 实验内容一

1. 在tcp_sock.c中实现tcp_sock_read函数和tcp_sock_write函数
2. 在tcp_in.c中增加对收取数据包的处理
3. 在节点h1上运行 `./tcp_stack server 10001` 启动TCP协议栈服务器模式
4. 在节点h2上运行  `./tcp_stack client 10.0.0.1 10001` 启动TCP协议栈客户端模式
5. client端向server端发送数据，server端收到数据后echo给client端
6. 保持节点h2不变，将节点h1的程序替换为tcp_stack.py `python tcp_stack.py server 10001` 再次进行实验
7. 保持节点h1不变，将节点h2的程序替换为tcp_stack.py `python tcp_stack.py client 10.0.0.1 10001` 再次进行实验



#### 实验内容二

1. 修改tcp_apps.c，tcp_stack.py
2. 执行 `./create_randfile.sh` 生成待传输的数据文件client-input.dat
3. 在节点h1上运行 `./tcp_stack server 10001` 启动TCP协议栈服务器模式
4. 在节点h2上运行  `./tcp_stack client 10.0.0.1 10001` 启动TCP协议栈客户端模式
5. client端向server端发送数据，server端收到数据后存储到server-output.dat
6. 保持节点h2不变，将节点h1的程序替换为tcp_stack.py `python tcp_stack.py server 10001` 再次进行实验
7. 保持节点h1不变，将节点h2的程序替换为tcp_stack.py `python tcp_stack.py client 10.0.0.1 10001` 再次进行实验
8. 每次实验server端完成数据接收后，执行 `md5sum server-output.dat` 和 `md5sum client-input.dat` 比较两个文件是否完全相同





### 四、实验结果

#### 实验内容一

1. **h1和h2均为tcp_stack.c程序**

<img src="/exp1/1/h1.jpg" alt="h1" style="zoom:80%;" />

<p align="center"> h1 output </p>

<img src="/exp1/1/h2.jpg" alt="h2" style="zoom:80%;" />

<p align="center"> h2 output </p>



2. **h1为tcp_stack.py程序，h2为tcp_stack.c程序**

<img src="/exp1/2/h1-py.jpg" alt="h1-py" style="zoom:80%;" />

<p align="center"> h1 output </p>

<img src="/exp1/2/h2.jpg" alt="h2" style="zoom:80%;" />

<p align="center"> h2 output </p>



3. **h1为tcp_stack.c程序，h2为tcp_stack.py程序**

<img src="/exp1/3/h1.jpg" alt="h1" style="zoom:80%;" />

<p align="center"> h1 output </p>

<img src="/exp1/3/h2-py.jpg" alt="h2-py" style="zoom:80%;" />

<p align="center"> h2 output </p>



#### 实验内容二

1. **h1和h2均为tcp_stack.c程序**

<img src="/exp2/1/h1.jpg" alt="h1" style="zoom:80%;" />

<p align="center"> h1 output </p>

<img src="/exp2/1/h2.jpg" alt="h2" style="zoom:80%;" />

<p align="center"> h2 output </p>



2. **h1为tcp_stack.py程序，h2为tcp_stack.c程序**

<img src="/exp2/2/h1-py.jpg" alt="h1-py" style="zoom:80%;" />

<p align="center"> h1 output </p>

<img src="/exp2/2/h2.jpg" alt="h2" style="zoom:80%;" />

<p align="center"> h2 output </p>



3. **h1为tcp_stack.c程序，h2为tcp_stack.py程序**

<img src="/exp2/3/h1.jpg" alt="h1" style="zoom:80%;" />

<p align="center"> h1 output </p>

<img src="/exp2/3/h2-py.jpg" alt="h2-py" style="zoom:80%;" />

<p align="center"> h2 output </p>





### 五、实验分析

#### 实验内容一

1. 观察实验结果，当 h1 和 h2 都使用我们自己编写的 tcp_stack.c 文件时，client 端成功发送数据，server 端成功接收到 client 端发送的数据并 echo 给了 client 端，并成功关闭 TCP 连接，实验成功
2. 观察实验结果，h1 使用 tcp_stack.py 程序，h2 使用 tcp_stack.c 程序，client 端成功发送数据，server 端成功接收到 client 端发送的数据并 echo 给了 client 端，并成功关闭 TCP 连接，实验成功
3. 观察实验结果，h1 使用 tcp_stack.c 程序，h2 使用 tcp_stack.py 程序，client 端成功发送数据，server 端成功接收到 client 端发送的数据并 echo 给了 client 端，并成功关闭 TCP 连接，实验成功
4. 当 h1 使用 tcp_stack.py 程序时，最后需要收到一个空数据包才能退出循环，这里我直接使用 `ctrl+c` 中断程序执行



#### 实验内容二

1. 观察实验结果，当 h1 和 h2 都使用我们自己编写的 tcp_stack.c 文件时，client 端成功发送数据，server 端成功接收到 client 端发送的数据并存储到 server-output.dat 文件中，并成功关闭TCP连接，使用 `md5sum` 程序检测收到的文件和发送的文件，结果相同，证明两个文件完全相同，实验成功
2. 观察实验结果，h1 使用 tcp_stack.py 程序，h2 使用 tcp_stack.c 程序，client 端成功发送数据，server 端成功接收到 client 端发送的数据并存储到 server-output.dat 文件中，并成功关闭 TCP 连接，使用 `md5sum` 程序检测收到的文件和发送的文件，结果相同，证明两个文件完全相同，实验成功
3. 观察实验结果，h1 使用 tcp_stack.c 程序，h2 使用 tcp_stack.py 程序，client 端成功发送数据，server 端成功接收到 client 端发送的数据并存储到 server-output.dat 文件中，并成功关闭 TCP 连接，使用 `md5sum` 程序检测收到的文件和发送的文件，结果相同，证明两个文件完全相同，实验成功
4. 当 h1 和 h2 都使用 tcp_stack.c 文件和 h1 使用 tcp_stack.py 程序，h2 使用 tcp_stack.c 程序时，客户端可以以较快速率发送数据
5. 当 h1 使用 tcp_stack.c 文件，h2 使用 tcp_stack.py 程序时，发送速率较低，且每次最多只能发送 500bytes 的数据，实验中，我使用的是 400bytes ，当一次发送的数据量超过 500bytes 时，或者发送速率过快时会产生丢包，主要现象是server端可能会出现只能读取 456byets 数据的情况





### 六、反思总结

1. 本次实验代码量较小，需要实验的内容也比较简单，但是还是碰到了一些比较刁钻的 bug，比如在 ring_buffer 结构体中添加了 lock 变量为互斥锁后，忘记了初始化 lock，导致后续的实验中进程卡死，耗费了较多时间；
2. 在实验二中，可能是由于最后的 server 进程没有正确执行到 tcp_close 和 fclose，导致我的最后一部分接收到的数据存留在缓冲区，而没有写入到 server-output.dat 文件中，耗费了我一个下午的时间，最后请老师帮忙查看，在 fwrite 函数后执行 fflush 函数将缓冲区的数据写入到 server-output.dat 中，成功解决了这个问题
3. 在 DEBUG 的时候，由于怀疑 server 程序有问题， 所以在 每次接受时加了一个 sleep 函数，结果导致可能会出现第一个发送的数据包丢失的现象出现
4. 当需要 sleep 小数秒时间的时候，需要采用 usleep 函数，如果采用 sleep 函数，貌似对睡眠时间小于 1 的处理是直接归零了，这也导致我一开始对发送速率的错误控制





### 七、参考文献

----

+ [1]  中国科学院大学 2020 春计算机网络研讨课 12-网络传输机制实验二实验课件

+ [2]  中国科学院大学 2020 春计算机网络研讨课 12-网络传输机制实验二实验附件



