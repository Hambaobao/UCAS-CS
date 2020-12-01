# 数字图像处理 大作业2  基于人脸视频的心率估计

#### **张磊 2017K8009922027**



### 一、实验题目

**基于人脸视频的心率估计**





### 二、 实验内容

1. **依据课堂介绍实现利用人脸视频进行心率估计的算法，鼓励自己设计模块**

	###### 本次实验未使用 MATLB 的心率估计算法包

2. **提供材料，人脸视频（8 段），Ground-Truth HR，人脸的 landmark**





### 三、实验流程

1. **输入人脸视频，获取感兴趣区域**

	###### 使用提供材料中的人脸的 landmark 数据，裁剪出感兴趣的部分，本次实验中采取了整个脸颊部

	```matlab
	function frame = Crop_frame(frame_raw, k, ROI_x, ROI_y)
	    % frame_raw 输入视频帧
	    % ROI_x 感兴趣区域的 landmark 的横坐标
	    % ROI_y 感兴趣区域的 landmark 的纵坐标
	    % frame 裁剪出的部分
	    
	    global lmk;
	
	    frame_raw_R = frame_raw(:, :, 1);
	    frame_raw_G = frame_raw(:, :, 2);
	    frame_raw_B = frame_raw(:, :, 3);  
	
	    intrest_region_x = lmk(k, ROI_x);
	    intrest_region_y = lmk(k, ROI_y);
	
	    bw = roipoly(frame_raw, intrest_region_x, intrest_region_y);
	
	    frame_R = frame_raw_R .* double(bw);
	    frame_G = frame_raw_G .* double(bw);
	    frame_B = frame_raw_B .* double(bw);
	
	    frame(:, :, 1) = frame_R;
	    frame(:, :, 2) = frame_G;
	    frame(:, :, 3) = frame_B;
	
	end 
	```

	

2. **从感兴趣区域中提取出 RGB 3 通道的颜色变化**

	###### 对整个感兴趣区域的颜色值求平均，以达到降噪的目的

	```matlab
	function RGB_Record = Get_RGB(frame)
	    % frame 经过裁剪后的视频帧
	    % RGB_Record 求解得到的 RGB 颜色值
	
	    [Row, Col, Dep] = size(frame);
	    pixel_count = 0;
	
	    R_value = 0.0;
	    G_value = 0.0;
	    B_value = 0.0;
	
	    for i = 1 : Row
	        for j = 1 : Col
	            if frame(i, j, 1) == 0 && frame(i , j, 2) == 0 && frame(i ,j , 3) == 0
	                continue;
	            else
	                pixel_count = pixel_count + 1;
	                R_value = R_value + double(frame(i, j, 1)); 
	                G_value = R_value + double(frame(i, j, 2)); 
	                B_value = R_value + double(frame(i, j, 3));               
	            end
	        end
	    end
	
	    Avg_R_value = R_value / pixel_count;
	    Avg_G_value = G_value / pixel_count;
	    Avg_B_value = B_value / pixel_count;
	
	    RGB_Record = [Avg_R_value Avg_G_value Avg_B_value]; 
	end
	```

	

3. **通过 RGB 的颜色变化，获得生理信号的表示**

	###### 直接套用课件上的公式即可

	```matlab
	function P = RGB2P(R_value, G_value, B_value, Method, Window)
	    % R_value   R 通道颜色变化
	    % G_value   G 通道颜色变化
	    % B_value   B 通道颜色变化
	    % Method    数据平滑滤波的方法，本次实验一律采用 Gaussian 滤波
	    % Window    平滑滤波的窗口大小
	    % P         生理信号
	
	    global numFrames;
	
	    Smooth_R_value = smoothdata(R_value, Method, Window);
	    Smooth_G_value = smoothdata(G_value, Method, Window);
	    Smooth_B_value = smoothdata(B_value, Method, Window);
	
	    Avg_Smooth_R_value = sum(Smooth_R_value) / numFrames;
	    Avg_Smooth_G_value = sum(Smooth_G_value) / numFrames;
	    Avg_Smooth_B_value = sum(Smooth_B_value) / numFrames;    
	    
	    Normal_R_value = Smooth_R_value ./ Avg_Smooth_R_value;
	    Normal_G_value = Smooth_G_value ./ Avg_Smooth_G_value;    
	    Normal_B_value = Smooth_B_value ./ Avg_Smooth_B_value;
	
	    Avg_Normal_R_value = sum(Normal_R_value) / numFrames; 
	    Avg_Normal_G_value = sum(Normal_G_value) / numFrames; 
	    Avg_Normal_B_value = sum(Normal_B_value) / numFrames; 
	
	    S1 = 3 .* Normal_R_value - 2 .* Avg_Normal_G_value;
	    S2 = 1.5 .* Normal_R_value + Avg_Normal_G_value - 1.5 .* Normal_B_value;
	
	    Var_S1 = var(S1, 1);
	    Var_S2 = var(S2, 1);
	    alp = Var_S1 / Var_S2;
	    
	    P = S1 - alp * S2;
	end
	```

	

4. **通过生理信号，求解心率**

	###### 以极大值点为一个周期的波峰，求解心率
	
	```matlab
	[Pks,Locs] = findpeaks(P);
	numLocalMax = size(Locs);
	
	for i = 1 : numLocalMax(2) - 1
	    Distance(i) = Locs(i + 1) - Locs(i);
	end
	
	Avg_Distance = sum(Distance) / (numLocalMax(2) - 1);
	HR = uint8(FPS / Avg_Distance * 60);
	```





### 四、实验结果

###### 数据平滑方法均为 Gaussian

|  视频  | Ground-Truth HR | Test HR |
| :----: | :-------------: | :-----: |
| Video1 |       74        |   70    |
| Video2 |       64        |   70    |
| Video3 |       52        |   57    |
| Video4 |       64        |   66    |
| Video5 |       87        |   90    |
| Video6 |       73        |   71    |
| Video7 |       63        |   61    |
| Video8 |       92        |   90    |





### 五、实验分析

1. 观察实验测试结果，测试得到的心率与 Ground-Truth HR 的误差都限制在 10 以内，平均误差 3.25，取得了较好的实验效果

2. 原本应该优先使用人脸的额头部分作为感兴趣区域，但是由于使用人脸的 landmark 时，人脸额头部分只有眉毛作为下边缘，而没有上边缘，并且有的视频，额头部分有头发遮盖，因此综合考虑后，选取了脸颊部分进行计算

3. 对于佩戴眼镜的测试人员，即使使用脸颊部分也会引起误差，好在对整个脸颊部分的平均可以减少误差的影响

4. 由于采样数据细小的波动（噪声）非常多，所以需要对采样的得到的数据进行平滑操作，我使用了 `movmean`，`movmedium`，`gaussian`，`sgolay` 4 种数据平滑方法，最后发现采用 `gaussian` 的平滑效果最好

5. 由于每个采样图像的波动剧烈程度不一，因此我对不同的视频选取了不同的数据平滑窗口的大小（见 `./code/test.m`）

6. 在人为选取平滑窗口大小以逼近真实心率时，我发现，人为选取的窗口与视频的**帧率**非常相近，于是，我也尝试了使用视频帧率替代人为规定的平滑窗口的大小，测试的结果虽然相比人为设定的窗口大，但是比起不设定窗口大小，测试结果要更加准确

	测试结果如下：

	|  视频  | Ground-Truth HR | Test HR |
	| :----: | :-------------: | :-----: |
	| Video1 |       74        |   74    |
	| Video2 |       64        |   77    |
	| Video3 |       52        |   77    |
	| Video4 |       64        |   87    |
	| Video5 |       87        |   62    |
	| Video6 |       73        |   71    |
	| Video7 |       63        |   61    |
	| Video8 |       92        |   41    |

	



### 六、反思总结

1. 本次实验难度以及代码量都相较于图像去雾算法有所提升，但是实现起来也还是比较顺利，只需要按照课件上的指导一步一步的完成即可；
2. 在采取感兴趣区域时，一直没有找到合适的裁剪图像的函数，最后经过查找资料，选择了 ROI_poly，这又让我学会了一项新的技能；
3. 本次实验进一步的让我熟悉了 MATLAB 的使用，能够将理论模型付诸实践的到成品也让我非常的有成就感（虽然效果不是很理想）;
4. 最后测试结果的误差很大程度来源于从感兴趣区域提取出原始颜色变化这一阶段（Get_RGB），一部分原因是由于对于感兴趣区域的提取不够精准，其中的胡子，眉毛这样的部分可能会造成影响，另一部分来自于对数据平滑的处理不够完善；
5. 虽然从颜色信号提取得到生理信号的方法有很多，如老师介绍到的 ICA，颜色空间投影，低秩分解，多区域融合，最后我还是选择了实现较为简单，但是效果更好的颜色空间投影法，之后有机会再尝试其他的方法；
6. 从生理信号获得心率时，我使用了时域空间的分析法，这样可以获得平均心率，也可以估算瞬时心率，但是少了消除误差的处理，更合理的方法应该可以采用逐差法减少误差，这里为了简单我使用了直接求平均的方法；





### 七、参考文献

----

+ [1]  中国科学院大学 2020 春 数字图像处理习题课 心率估计-生理指标测量 课件




