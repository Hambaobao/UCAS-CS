f_taildrop = load("../taildrop/get_ping.txt");
f_red = load("../red/get_ping.txt");
f_codel = load("../codel/get_ping.txt");

ping_taildrop = f_taildrop(:,1);
ping_red = f_red(:,1);
ping_codel = f_codel(:,1);

hold on

plot(ping_taildrop, "-r")
plot(ping_red, "-b")
plot(ping_codel, "-m")

title("Time  -----  Ping")
legend("Taildrop", "Red", "Codel")
set(get(gca, 'XLabel'), 'String', 'Time/s');
set(get(gca, 'YLabel'), 'String', 'Ping/ms');
set(gca, 'XLim', [0,60])
set(gca, 'FontSize', 15)

hold off

saveas(gcf, "../res_pic/BufferBloat_Ping", "png");
