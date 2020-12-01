clc;
clear all;
close all;

J = imread('../Org_images/canon3.bmp');
J = double(J);
J = J ./ 255 ;

figure(1); imshow(J);

% 求暗通道图像 Jdark = min(min());
Jdark = Idark(J);
figure(2); imshow(Jdark,[]);
 

% 注意：何凯明使用了soft matting方法对得到的粗透射率Jt进行细化 
%       本代码采用梯度导向滤波实现
Jdark = GradientGuidedFilter(Jdark,Jdark, 0.04);
figure(3); imshow(Jdark,[]);


% 大气物理模型 J = I*t + A*(1-t)  【直接衰减项】+【大气光照】
% 透射率 t与深度的关系 t = exp(-a*depth)
w = 0.95;           %雾的保留系数
Jt = 1 - w * Jdark; %求解透射率


% 求解全局大气光照
% 1.首先对输入的有雾图像I求解其暗通道图像Jdark。
% 2.选择Jdark总像素点个数千分之一（N/1000）个最亮的像素点，记录像素点（x,y）坐标
% 3.根据点的坐标分别在原图像J的三个通道（r,g,b）内找到这些像素点并加和得到（sum_r,sum_g,sum_b）.
% 4.Ac=[Ar,Ag,Ab]. 其中Ar=sum_r/N;   Ag=sum_g/N;   Ab=sum_b/N.
[m, n, ~] = size(J);
N = floor(m * n ./ 1000);
MaxPos = [0, 0]; % 初始化
for i = 1 : 1 : N
    MaxValue = max(max(Jdark));
    [x,y] = find(Jdark == MaxValue);
    Jdack(Jdark == MaxValue) = 0; %最大值置零，寻找下一次次大值
    %检查长度
    MaxPos = vertcat(MaxPos, [x, y]);
    Cnt = length(MaxPos(1));
    if Cnt > N
        break;
    end
end

MaxPosN = MaxPos(2 : N + 1, :);
 
Rsum = 0;  Jr = J(:, :, 1);
Gsum = 0;  Jg = J(:, :, 2);
Bsum = 0;  Jb = J(:, :, 3);
for j = 1 : 1 : N
    Rsum = Rsum + Jr(MaxPosN(j, 1), MaxPosN(j, 2));
    Gsum = Gsum + Jg(MaxPosN(j, 1), MaxPosN(j, 2));
    Bsum = Bsum + Jb(MaxPosN(j, 1), MaxPosN(j, 2));
end

Ac = [Rsum / N, Gsum / N, Bsum / N];


% 求解清晰的图像
% 根据 J = I*t + A*(1-t)   I = (J-A)/Jt + A
Iorg = zeros(m, n, 3);
for i = 1 : 1 : m
    for j = 1 : 1 : n
        for k = 1 : 1 : 3
        Iorg(i, j, k) = (J(i, j, k) - Ac(k)) ./ Jt(i, j) + Ac(k);
        end
    end
end

figure(4); imshow(Iorg, []);