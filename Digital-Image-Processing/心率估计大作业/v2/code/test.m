numVideos = 8;
Windows = [50, 50, 80, 70, 15, 30, 30, 15];

for i = 1 : numVideos
    fprintf("Processing The %dth Video\n", i);
    Path = strcat('../videos/video', num2str(i));
    [Golden_HR, HR] = Check(Path, Windows(i));
    Golden_Trace(i) = Golden_HR;
    Test_HR(i) = HR;
end

fprintf("\n========================Results======================\n");

for i = 1 : numVideos
    fprintf("Video %d:\n", i);
    fprintf("Golden Heart Rate: %d\n", Golden_Trace(i));
    fprintf("Test Heart Rate: %d\n", Test_HR(i));
    fprintf("\n");
end