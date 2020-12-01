function [Golden_HR, HR] = Check(Path, Window)

    global ROI_forehead_x ROI_forehead_y;
    global ROI_left_cheek_x ROI_left_cheek_y;

    ROI_forehead_x = [28 22 21 20 24 23];
    ROI_forehead_y = ROI_forehead_x + 68;

    ROI_left_cheek_x = [2 3 4 5 32 41];
    ROI_left_cheek_y = ROI_left_cheek_x + 68;

    ROI_right_cheek_x = [13 14 15 16 17 36];
    ROI_right_cheek_y = ROI_right_cheek_x + 68;  

    ROI_full_cheek_x = [2 3 4 5 32 31 36 13 14 15 16 47 29 41];
    ROI_full_cheek_y = ROI_full_cheek_x + 68;

    golden_hr_path = strcat(Path, '/HR_gt.txt');
    Golden_HR = textread(golden_hr_path);

    fps_path = strcat(Path, '/fps.txt');
    FPS = textread(fps_path);

    global lmk
    lmk_path = strcat(Path, '/lmk.csv');
    lmk = csvread(lmk_path, 1, 5);

    %输入视频位置
    video_path = strcat(Path, '/video.avi');
    obj = VideoReader(video_path);

    % 帧的总数
    global numFrames;
    numFrames = obj.NumberOfFrames;

    % 读取每一帧
    for k = 1 : numFrames
        % fprintf("Processing The %dth Frame\n", k);

        %读取第k帧    
        frame_raw = double(read(obj,k));

        % frame_left_cheek = Crop_frame(frame_raw, k, ROI_left_cheek_x, ROI_left_cheek_y);
        % frame_right_cheek = Crop_frame(frame_raw, k, ROI_right_cheek_x, ROI_right_cheek_y);
        frame_full_cheek = Crop_frame(frame_raw, k, ROI_full_cheek_x, ROI_full_cheek_y);
        % frame_forehead = Crop_frame(frame_raw, k, ROI_forehead_x, ROI_forehead_y);

        % RGB_Record_left_cheek = Get_RGB(frame_left_cheek);
        % RGB_Record_right_cheek = Get_RGB(frame_right_cheek);        
        RGB_Record_full_cheek = Get_RGB(frame_full_cheek);
        % RGB_Record_forehead = Get_RGB(frame_forehead);


        % R_value(k) = (RGB_Record_left_cheek(1) + RGB_Record_right_cheek(1)) / 2;
        % G_value(k) = (RGB_Record_left_cheek(2) + RGB_Record_right_cheek(2)) / 2;
        % B_value(k) = (RGB_Record_left_cheek(3) + RGB_Record_right_cheek(3)) / 2;      
        R_value(k) = RGB_Record_full_cheek(1);
        G_value(k) = RGB_Record_full_cheek(2);
        B_value(k) = RGB_Record_full_cheek(3); 
        % R_value(k) = RGB_Record_forehead(1);
        % G_value(k) = RGB_Record_forehead(2);
        % B_value(k) = RGB_Record_forehead(3); 

    end

    P = RGB2P(R_value, G_value, B_value, 'gaussian', Window);
    % P = RGB2P(R_value, G_value, B_value, 'gaussian', FPS);
    
    [Pks,Locs] = findpeaks(P);
    numLocalMax = size(Locs);

    for i = 1 : numLocalMax(2) - 1
        Distance(i) = Locs(i + 1) - Locs(i);
    end

    Avg_Distance = sum(Distance) / (numLocalMax(2) - 1);
    HR = uint8(FPS / Avg_Distance * 60);

    % fprintf("Golgen Heart Rate: %d\n", Golden_HR);
    % fprintf("Heart Rate: %d\n", HR);

    % hold on
    % plot(P, 'm');  
    % set(get(gca, 'Title'), 'String', 'Heart Rate Evaluate'); 
    % set(get(gca, 'XLabel'), 'String', 'Frame');
    % set(get(gca, 'YLabel'), 'String', 'Target Value');    
    % hold off

end