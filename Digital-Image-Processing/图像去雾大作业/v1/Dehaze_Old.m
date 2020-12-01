I = imread("../Org_images/canon3.bmp");

[Row, Col, Dim] = size(I);

Eps = 1;
for i = 1 : Row
    for j = 1 : Col
        for k = 1 : Dim
            localmin = I(i, j, k);
            for r = - Eps : Eps
                for c = - Eps : Eps
                    if(r ~= 0 || c ~= 0)
                        if(i + r > 0 && i + r <= Row && j + c > 0 && j + c <= Col)
                            localmin = min(localmin, I(i + r, j + c, k));
                        end
                    end
                end
            end
            Dark(k) = localmin;
        end
        Jdark(i, j) = min(Dark);
    end
end


CountPixel = 0;
JdarkMax = max(max(Jdark));
while(CountPixel < Row * Col / 1000)
    for i = 1 : Row
        for j = 1 : Col
            if(Jdark(i ,j) == JdarkMax)
                CountPixel = CountPixel + 1;
                Cord_x(CountPixel) = i;
                Cord_y(CountPixel) = j;
            end
        end
    end
    JdarkMax = JdarkMax - 1;
end


sum_r = 0;
sum_g = 0;
sum_b = 0;
for i = 1 : CountPixel
    sum_r = sum_r + double(I(Cord_x(i), Cord_y(i), 1));
    sum_g = sum_g + double(I(Cord_x(i), Cord_y(i), 2));
    sum_b = sum_b + double(I(Cord_x(i), Cord_y(i), 3));
end
A = [sum_r / CountPixel, sum_g / CountPixel, sum_b / CountPixel];


for i = 1 : Row
    for j = 1 : Col
        Count = 0;
        for r = - Eps : Eps
            for c = - Eps : Eps
                for k = 1 : Dim
                    if(i + r > 0 && i + r <= Row && j + c > 0 && j + c <= Col)
                        Count = Count + 1;
                        InnerMin(Count) = double(I(i + r, j + c, k)) / A(k);
                    end
                end
            end
        end
        T(i, j) = 1 - 0.95 * min(InnerMin);
    end
end


for i = 1 : Row
    for j = 1 : Col 
        for k = 1 : Dim
            J(i, j, k) = uint8((I(i, j, k) - A(k)) / max(T(i, j), 0.1) + A(k));
        end
    end
end

imshow(J);