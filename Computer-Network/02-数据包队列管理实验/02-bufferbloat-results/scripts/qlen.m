f10_qlen = load("../qlen-10/get_qlen.txt");
%f20_qlen = load("../qlen-20/get_qlen.txt");
f30_qlen = load("../qlen-30/get_qlen.txt");
%f40_qlen = load("../qlen-40/get_qlen.txt");
f50_qlen = load("../qlen-50/get_qlen.txt");
%f60_qlen = load("../qlen-60/get_qlen.txt");
f70_qlen = load("../qlen-70/get_qlen.txt");
%f80_qlen = load("../qlen-80/get_qlen.txt");
f90_qlen = load("../qlen-90/get_qlen.txt");
%f100_qlen = load("../qlen-100/get_qlen.txt");

time10 = f10_qlen(:,1) - f10_qlen(1, 1);
%time20 = f20_qlen(:,1) - f20_qlen(1, 1);
time30 = f30_qlen(:,1) - f30_qlen(1, 1);
%time40 = f40_qlen(:,1) - f40_qlen(1, 1);
time50 = f50_qlen(:,1) - f50_qlen(1, 1);
%time60 = f60_qlen(:,1) - f60_qlen(1, 1);
time70 = f70_qlen(:,1) - f70_qlen(1, 1);
%time80 = f80_qlen(:,1) - f80_qlen(1, 1);
time90 = f90_qlen(:,1) - f90_qlen(1, 1);
%time100 = f100_qlen(:,1) - f100_qlen(1, 1);

qlen10 = f10_qlen(:,2);
%qlen20 = f20_qlen(:,2);
qlen30 = f30_qlen(:,2);
%qlen40 = f40_qlen(:,2);
qlen50 = f50_qlen(:,2);
%qlen60 = f60_qlen(:,2);
qlen70 = f70_qlen(:,2);
%qlen80 = f80_qlen(:,2);
qlen90 = f90_qlen(:,2);
%qlen100 = f100_qlen(:,2);

hold on

plot(time10, qlen10, "-r");
%plot(time20, qlen20, "-g");
plot(time30, qlen30, "-g");
%plot(time40, qlen40, "-c");
plot(time50, qlen50, "-b");
%plot(time60, qlen60, "-y");
plot(time70, qlen70, "-c");
%plot(time80, qlen80, "-w");
plot(time90, qlen90, "-m");
%plot(time100, qlen100, "-g");

title("Time  -----  Qlen")
legend("MAXQ==10", "MAXQ==30", "MAXQ==50", "MAXQ==70","MAXQ==90")
set(get(gca, 'XLabel'), 'String', 'Time/s');
set(get(gca, 'YLabel'), 'String', 'qlen');
set(gca, 'YLim', [0,100])
set(gca, 'FontSize', 15)

hold off

saveas(gca, "../res_pic/qlen", "png");
