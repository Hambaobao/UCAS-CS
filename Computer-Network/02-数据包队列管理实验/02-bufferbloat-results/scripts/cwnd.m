f10_cwnd = load("../qlen-10/get_cwnd.txt");
%f20_cwnd = load("../qlen-20/get_cwnd.txt");
f30_cwnd = load("../qlen-30/get_cwnd.txt");
%f40_cwnd = load("../qlen-40/get_cwnd.txt");
f50_cwnd = load("../qlen-50/get_cwnd.txt");
%f60_cwnd = load("../qlen-60/get_cwnd.txt");
f70_cwnd = load("../qlen-70/get_cwnd.txt");
%f80_cwnd = load("../qlen-80/get_cwnd.txt");
f90_cwnd = load("../qlen-90/get_cwnd.txt");
%f100_cwnd = load("../qlen-100/get_cwnd.txt");


time10 = f10_cwnd(:,1);
%time20 = f20_cwnd(:,1);
time30 = f30_cwnd(:,1);
%time40 = f40_cwnd(:,1);
time50 = f50_cwnd(:,1);
%time60 = f60_cwnd(:,1);
time70 = f70_cwnd(:,1);
%time80 = f80_cwnd(:,1);
time90 = f90_cwnd(:,1);
%time100 = f100_cwnd(:,1);


[cwnd10, time10_a] = findpeaks(f10_cwnd(:,4));
%[cwnd20, time20_a] = findpeaks(f20_cwnd(:,4));
[cwnd30, time30_a] = findpeaks(f30_cwnd(:,4));
%[cwnd40, time40_a] = findpeaks(f40_cwnd(:,4));
[cwnd50, time50_a] = findpeaks(f50_cwnd(:,4));
%[cwnd60, time60_a] = findpeaks(f60_cwnd(:,4));
[cwnd70, time70_a] = findpeaks(f70_cwnd(:,4));
%[cwnd80, time80_a] = findpeaks(f80_cwnd(:,4));
[cwnd90, time90_a] = findpeaks(f90_cwnd(:,4));
%[cwnd100, time100_a] = findpeaks(f100_cwnd(:,4));

rtime10 = time10(time10_a);
%rtime20 = time20(time20_a);
rtime30 = time30(time30_a);
%rtime40 = time40(time40_a);
rtime50 = time50(time50_a);
%rtime60 = time60(time60_a);
rtime70 = time70(time70_a);
%rtime80 = time80(time80_a);
rtime90 = time90(time90_a);
%rtime100 = time100(time100_a);

hold on

plot(rtime10, cwnd10, "-r");
%plot(rtime20, cwnd20, "-g");
plot(rtime30, cwnd30, "-g");
%plot(rtime40, cwnd40, "-c");
plot(rtime50, cwnd50, "-b");
%plot(rtime60, cwnd60, "-y");
plot(rtime70, cwnd70, "-c");
%plot(rtime80, cwnd80, "-w");
plot(rtime90, cwnd90, "-m");
%plot(rtime100, cwnd100, "-g");
 

title("Time  -----  Cwnd")
legend("MAXQ==10", "MAXQ==30", "MAXQ==50", "MAXQ==70","MAXQ==90")
set(get(gca, 'XLabel'), 'String', 'Time/s');
set(get(gca, 'YLabel'), 'String', 'Cwnd');
set(gca, 'XLim', [0,60])
set(gca, 'FontSize', 15)

hold off

saveas(gca, "../res_pic/cwnd", "png");
