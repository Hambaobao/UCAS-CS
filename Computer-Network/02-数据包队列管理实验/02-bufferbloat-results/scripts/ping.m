f10_ping = load("../qlen-10/get_ping.txt");
%f20_ping = load("../qlen-20/get_ping.txt");
f30_ping = load("../qlen-30/get_ping.txt");
%f40_ping = load("../qlen-40/get_ping.txt");
f50_ping = load("../qlen-50/get_ping.txt");
%f60_ping = load("../qlen-60/get_ping.txt");
f70_ping = load("../qlen-70/get_ping.txt");
%f80_ping = load("../qlen-80/get_ping.txt");
f90_ping = load("../qlen-90/get_ping.txt");
%f100_ping = load("../qlen-100/get_ping.txt");

ping10 = f10_ping(:,1);
%ping20 = f20_ping(:,1);
ping30 = f30_ping(:,1);
%ping40 = f40_ping(:,1);
ping50 = f50_ping(:,1);
%ping60 = f60_ping(:,1);
ping70 = f70_ping(:,1);
%ping80 = f80_ping(:,1);
ping90 = f90_ping(:,1);
%ping100 = f100_ping(:,1);

hold on

plot(ping10, "-r");
%plot(ping20, "-g");
plot(ping30, "-g");
%plot(ping40, "-m");
plot(ping50, "-b");
%plot(ping60, "-g");
plot(ping70, "-c");
%plot(ping80, "-m");
plot(ping90, "-m");
%plot(ping100, "-m");

title("Time  -----  Ping")
legend("MAXQ==10", "MAXQ==30", "MAXQ==50", "MAXQ==70","MAXQ==90")
set(get(gca, 'XLabel'), 'String', 'Time/0.1s');
set(get(gca, 'YLabel'), 'String', 'ping');
set(gca, 'FontSize', 15)

hold off

saveas(gca, "../res_pic/ping", "png");
