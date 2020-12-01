band_width   = [10, 50, 100, 500, 1000]; % Mbps

filesize_1MB_time      = [0.9, 0.2, 0.2, 0.1, 0.1];    % s
filesize_10MB_time     = [8.8, 1.8, 1.1, 0.4, 0.4];
filesize_100MB_time    = [88, 18, 10, 0.25, 1.4];

filesize_1MB_velo      = [1.12, 4.22, 5.89, 6.89, 7.39];         % MB/s
filesize_10MB_velo     = [1.14, 5.44, 9.07, 26.5, 26.3];
filesize_100MB_velo    = [1.14, 5.61, 9.88, 40.7, 72.7];


subplot(1, 2, 1);

hold on
plot(band_width, filesize_1MB_time, '-or', 'LineWidth', 1.5, ...
     'MarkerFaceColor', 'g', 'MarkerEdgeColor', 'c');
plot(band_width, filesize_10MB_time, '-sm', 'LineWidth', 1.5, ...
     'MarkerFaceColor', 'b', 'MarkerEdgeColor', 'c');
plot(band_width, filesize_100MB_time, '-pg', 'LineWidth', 1.5, ...
     'MarkerFaceColor', 'c', 'MarkerEdgeColor', 'r');
hold off

title("FileSize --- Time");
set(gca, 'FontSize', 13);
set(gca, 'XTick', 0 : 200 : 1000);
set(get(gca, 'XLabel'), 'String', 'BandWith / Mbps');
set(get(gca, 'YLabel'), 'String', 'Time / s');
legend("1MB", "10MB", "100MB", "location", "northeast");

subplot(1, 2, 2);

hold on
plot(band_width, filesize_1MB_velo, '-or', 'LineWidth', 1.5, ...
     'MarkerFaceColor', 'g', 'MarkerEdgeColor', 'c');
plot(band_width, filesize_10MB_velo, '-sm', 'LineWidth', 1.5, ...
     'MarkerFaceColor', 'b', 'MarkerEdgeColor', 'c');
plot(band_width, filesize_100MB_velo, '-pg', 'LineWidth', 1.5, ...
     'MarkerFaceColor', 'c', 'MarkerEdgeColor', 'r');
hold off

title("FileSize --- Velocity");
set(gca, 'FontSize', 13);
set(gca, 'XTick', 0 : 200 : 1000);
set(get(gca, 'XLabel'), 'String', 'BandWith / Mbps');
set(get(gca, 'YLabel'), 'String', 'Velocity / MB/s');
legend("1MB", "10MB", "100MB", "location", "northwest");
