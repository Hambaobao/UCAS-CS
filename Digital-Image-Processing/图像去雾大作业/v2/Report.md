---
typora-root-url: ./assets
---

# 数字图像处理 大作业1  图像去雾算法

#### **张磊 2017K8009922027**



### 一、实验题目

​	**图像去雾算法**





### 二、 实验内容

​	**结合课堂教学，利用何凯明博士的基于暗通道的算法，实现暗通道图像去雾**





### 三、实验流程

1. 获取输入图像的 Jdark 图像

	```matlab
	function Jdark = Idark(I, Eps)
	    % input: I:输入图像, Eps:邻域窗口
	    % output: Jdark = min(min(r),min(g),min(b));
	    [Row, Col, Dim] = size(I);
	
	    % 暗通道
	    for i = 1 : Row
	        for j = 1 : Col
	            for k = 1 : Dim
	                localmin = I(i, j, k);
	                for r = - Eps : Eps
	                    for c = - Eps : Eps
	                        if(i + r > 0 && i + r <= Row && j + c > 0 && j + c <= Col)
	                            localmin = min(localmin, I(i + r, j + c, k));
	                        end
	                    end
	                end
	                Dark(k) = localmin;
	            end
	            Jdark(i, j) = min(Dark);
	        end
	    end
	end
	```

	

2. 对 Jdark 图像进行导向滤波

	```matlab
	function q = GradientGuidedFilter(I, p, eps)  
	    %   GUIDEDFILTER   O(1) time implementation of guided filter.  
	    %  
	    %   - guidance image: I (should be a gray-scale/single channel image)  
	    %   - filtering input image: p (should be a gray-scale/single channel image)  
	    %   - regularization parameter: eps  
	      
	    r = 16;  
	    [hei, wid] = size(I);  
	    N = Boxfilter(ones(hei, wid), r); % the size of each local patch; N=(2r+1)^2 except for boundary pixels.  
	      
	    mean_I = Boxfilter(I, r) ./ N;  
	    mean_p = Boxfilter(p, r) ./ N;  
	    mean_Ip = Boxfilter(I .* p, r) ./ N;  
	    cov_Ip = mean_Ip - mean_I .* mean_p; % this is the covariance of (I, p) in each local patch.  
	      
	    mean_II = Boxfilter(I .* I, r) ./ N;  
	    var_I = mean_II - mean_I .* mean_I;  
	      
	    %weight  
	    epsilon = (0.001 * (max(p(:)) - min(p(:)))) ^ 2;  
	    r1 = 1;  
	      
	    N1 = Boxfilter(ones(hei, wid), r1); % the size of each local patch; N=(2r+1)^2 except for boundary pixels.  
	    mean_I1 = Boxfilter(I, r1) ./ N1;  
	    mean_II1 = Boxfilter(I .* I, r1) ./ N1;  
	    var_I1 = mean_II1 - mean_I1 .* mean_I1;  
	      
	    chi_I = sqrt(abs(var_I1 .* var_I));      
	    weight = (chi_I + epsilon) / (mean(chi_I(:)) + epsilon);       
	      
	    gamma = (4 / (mean(chi_I(:)) - min(chi_I(:)))) * (chi_I - mean(chi_I(:)));  
	    gamma = 1 - 1 ./ (1 + exp(gamma));  
	      
	    %result  
	    a = (cov_Ip + (eps ./ weight) .* gamma) ./ (var_I + (eps ./ weight));   
	    b = mean_p - a .* mean_I;   
	      
	    mean_a = Boxfilter(a, r) ./ N;  
	    mean_b = Boxfilter(b, r) ./ N;  
	      
	    q = mean_a .* I + mean_b; 
	end  	
	```

	```matlab
	function imDst = Boxfilter(imSrc, r)  
	    %   BOXFILTER   O(1) time box filtering using cumulative sum  
	    %  
	    %   - Definition imDst(x, y)=sum(sum(imSrc(x-r:x+r,y-r:y+r)));  
	    %   - Running time independent of r;   
	    %   - Equivalent to the function: colfilt(imSrc, [2*r+1, 2*r+1], 'sliding', @sum);  
	    %   - But much faster.  
	  
	    [hei, wid] = size(imSrc);  
	    imDst = zeros(size(imSrc));  
	    
	    %cumulative sum over Y axis  
	    imCum = cumsum(imSrc, 1);  
	    %difference over Y axis  
	    imDst(1 : r + 1, :) = imCum(1 + r : 2 * r + 1, :);  
	    imDst(r + 2 : hei - r, :) = imCum(2 * r + 2 : hei, :) - imCum(1 : hei - 2 * r - 1, :);  
	    imDst(hei - r + 1 : hei, :) = repmat(imCum(hei, :), [r, 1]) - imCum(hei - 2 * r : hei - r - 1, :);  
	    
	    %cumulative sum over X axis  
	    imCum = cumsum(imDst, 2);  
	    %difference over X axis  
	    imDst(:, 1 : r + 1) = imCum(:, 1 + r : 2 * r + 1);  
	    imDst(:, r + 2 : wid - r) = imCum(:, 2 * r + 2 : wid) - imCum(:, 1 : wid - 2 * r - 1);  
	    imDst(:, wid - r + 1 : wid) = repmat(imCum(:, wid), [1, r]) - imCum(:, wid - 2 * r : wid - r - 1);  
	
	end
	```

	

3. 求解全局大气光照 A

	```matlab
	    % 求解全局大气光照
	    % 1. 首先对输入的有雾图像 I 求解其暗通道图像 Jdark。
	    % 2. 选择Jdark总像素点个数千分之一（N / 1000）个最亮的像素点，记录像素点（x, y）坐标
	    % 3. 根据点的坐标分别在原图像J的三个通道（r, g, b）内找到这些像素点并加和得到（sum_r, sum_g, sum_b）.
	    % 4. A = [Ar, Ag, Ab]. 其中 Ar = sum_r / N;   Ag = sum_g / N;   Ab = sum_b / N.
	    
	    [Row, Col, Dim] = size(I);
	    N = 0;
	    while(N < Row * Col / 1000)
	        JdarkMax = max(max(Jdark));
	        for i = 1 : 1 : Row
	            for j = 1 : 1 : Col
	                if(Jdark(i ,j) == JdarkMax)
	                    N = N + 1;
	                    Cord_x(N) = i;
	                    Cord_y(N) = j;
	                    Jdark(i ,j) = 0;
	                end
	            end
	        end
	    end
	
	    if(Dim == 3)
	        sum_r = 0;
	        sum_g = 0;
	        sum_b = 0;
	        for i = 1 : 1 : N
	            sum_r = sum_r + I(Cord_x(i), Cord_y(i), 1);
	            sum_g = sum_g + I(Cord_x(i), Cord_y(i), 2);
	            sum_b = sum_b + I(Cord_x(i), Cord_y(i), 3);
	        end
	        A = [sum_r / N, sum_g / N, sum_b / N];
	```

	

4. 求解透射率

	```matlab
	    % 大气物理模型 I = J * t + A * (1 - t)  【直接衰减项】+【大气光照】
	    % 透射率 t与深度的关系 t = exp(- a * depth)
	    w = 0.95;           %雾的保留系数
	    T = 1 - w * Jdark; %求解透射率
	```

	

5. 求解去雾图像

	```matlab
	    % 彩色图像
	    if(Dim == 3)
	        sum_r = 0;
	        sum_g = 0;
	        sum_b = 0;
	        for i = 1 : 1 : N
	            sum_r = sum_r + I(Cord_x(i), Cord_y(i), 1);
	            sum_g = sum_g + I(Cord_x(i), Cord_y(i), 2);
	            sum_b = sum_b + I(Cord_x(i), Cord_y(i), 3);
	        end
	        A = [sum_r / N, sum_g / N, sum_b / N];
	
	
	        % 求解清晰的图像
	        % I = J * t + A * (1 - t) =>  J = (I - A) / T + A
	        t0 = 0.1;    %噪声下界
	        J = zeros(Row, Col, 3);
	        for i = 1 : 1 : Row
	            for j = 1 : 1 : Col
	                J(i, j, 1) = (I(i, j, 1) - A(1)) ./ max(T(i, j), t0) + A(1);
	                J(i, j, 2) = (I(i, j, 2) - A(2)) ./ max(T(i, j), t0) + A(2);
	                J(i, j, 3) = (I(i, j, 3) - A(3)) ./ max(T(i, j), t0) + A(3);
	            end
	        end
	        figure(4); imshow(J, []);
		% 黑白图像
	    elseif(Dim == 1)
	        sum_ = 0;
	        for i = 1 : 1 : N
	            sum_ = sum_ + I(Cord_x(i), Cord_y(i), 1);
	        end
	        A = [sum_ / N];
	
	
	        % 求解清晰的图像
	        % I = J * t + A * (1 - t) =>  J = (I - A) / T + A
	        t0 = 0.1;    %噪声下界
	        J = zeros(Row, Col);
	        for i = 1 : 1 : Row
	            for j = 1 : 1 : Col
	                J(i, j) = (I(i, j) - A) ./ max(T(i, j), t0) + A;
	            end
	        end
	        figure(4); imshow(J, []);        
	    end
	```





### 四、实验结果

###### 	左图为未经过处理的图像，右图为经过去雾处理的图像

1. **测试图像 1**

	<img src="/../raw/1.bmp" style="zoom:80%;" />                                                                   <img src="/../res/1.png" alt="1" style="zoom:80%;" />



2. **测试图像 2**

	<img src="/../raw/2.bmp" alt="2" style="zoom:80%;" />                                               <img src="/../res/2.png" alt="2" style="zoom:80%;" />



3. **测试图像 3**

	<img src="/../raw/3.jpg" alt="3" style="zoom:40%;" />                        <img src="/../res/3.png" alt="3" style="zoom:40%;" />



4. **测试图像 4**

	<img src="/../raw/4.bmp" alt="4" style="zoom:80%;" />                                                                  <img src="/../res/4.png" alt="4" style="zoom:80%;" />



5. **测试图像 5**

	<img src="/../raw/5.bmp" alt="5" style="zoom:55%;" />         <img src="/../res/5.png" alt="5" style="zoom:55%;" />



6. **测试图像 6**

	<img src="/../raw/6.bmp" alt="6" style="zoom:55%;" />         <img src="/../res/6.png" alt="6" style="zoom:55%;" />



7. **测试图像 7**

	<img src="/../raw/7.bmp" alt="7" style="zoom:80%;" />                                                                 <img src="/../res/7.png" alt="7" style="zoom:80%;" />



8. **测试图像 8**

	<img src="/../raw/8.bmp" alt="8" style="zoom:67%;" />                          <img src="/../res/8.png" alt="8" style="zoom:67%;" />



9. **测试图像 9**

	<img src="/../raw/9.bmp" alt="9" style="zoom:67%;" />                          <img src="/../res/9.png" alt="9" style="zoom:67%;" />





### 五、实验分析

1. 观察实验结果，可以看到明显的去雾效果，但是相比于老师给出的图像去雾结果还有一定的差距
2. 由于没有对透射率进行微调，而是采用了对 Jdark 图像进行导向滤波的操作，导致如：测试图像 3，测试图像 4，测试图像 9 的物体的边缘出现白色轮廓
3. 色彩偏暗的图像的去雾效果比色彩偏亮的图像的去雾效果好
4. 即使不对 Jdark 图像进行导向滤波也可以达到较好的效果，在实际测试当中，除测试图像 3，4，9 外的其余图像，并无明显差别，不进行导向滤波时测试图像 3，4，9 白色边缘轮廓更加明显
5. 在求解 Jdark 图像时，是用较小的窗口大小可以获得更加细腻的 Jdark 图像，后期去雾的效果更好一些





### 六、反思总结

1. 本次大作业实现起来较为简单，只需要按照老师课上讲述的流程依次进行处理即可
2. 本次实验的对 Jdark 图像进行导向滤波的部分参考了网上的代码（见下文参考文献），原本不进行导向滤波的结果已经可以达到明显的去雾效果了，但是由于测试图像 3，4，9 的白色轮廓过于明显，所以又增加了对 Jdark 图像的导向滤波操作
3. 本次实验让我体会到了数字图像处理的乐趣，相比于用 Photoshop 之类的修图软件进行调色，我又掌握了一项去雾的方法，而且是我自己亲手编写的，让我非常的有成就感





### 七、参考文献

----

+ [1]  中国科学院大学 2020 春数字图像处理 习题课2-去雾算法 课件

+ [2]  [Jdark 图像导向滤波的实现](https://blog.csdn.net/shenziheng1/article/details/56951002)

