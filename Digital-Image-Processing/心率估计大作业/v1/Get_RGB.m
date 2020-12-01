function RGB_Record = Get_RGB(frame)
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

                % fprintf("R_value: %f\n", R_value);
                % fprintf("G_value: %f\n", G_value);
                % fprintf("B_value: %f\n", B_value);                
            end
        end
    end

    Avg_R_value = R_value / pixel_count;
    Avg_G_value = G_value / pixel_count;
    Avg_B_value = B_value / pixel_count;

    % Avg_R_value = round(R_value / pixel_count);
    % Avg_G_value = round(G_value / pixel_count);
    % Avg_B_value = round(B_value / pixel_count);

    % fprintf("R_value: %f\n", R_value);
    % fprintf("G_value: %f\n", G_value);
    % fprintf("B_value: %f\n", B_value);

    % fprintf("Pixel Count: %d\n", pixel_count);
    % fprintf("Avg_R_value: %d\n", Avg_R_value);
    % fprintf("Avg_G_value: %d\n", Avg_G_value);
    % fprintf("Avg_B_value: %d\n", Avg_B_value);

    RGB_Record = [Avg_R_value Avg_G_value Avg_B_value]; 
end