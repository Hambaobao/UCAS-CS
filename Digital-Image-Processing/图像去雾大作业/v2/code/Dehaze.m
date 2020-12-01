function J = Dehaze(ImagePath)

    I = imread(ImagePath);
    I = double(I);
    I = I ./ 255 ;
    % figure(1); imshow(I);


    % 求暗通道图像 Jdark = min(min());
    Eps = 1;    %邻域窗口
    Jdark = Idark(I, Eps);
    % figure(2); imshow(Jdark, []);

    
    Jdark = GradientGuidedFilter(Jdark,Jdark, 0.04);
    % figure(3); imshow(Jdark,[]);


    % 大气物理模型 I = J * t + A * (1 - t)  【直接衰减项】+【大气光照】
    % 透射率 t与深度的关系 t = exp(-a * depth)
    w = 0.95;           %雾的保留系数
    T = 1 - w * Jdark; %求解透射率


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
        % figure(3); imshow(J, []);

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
    end
    
end


