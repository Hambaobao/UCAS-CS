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