X = load('./14-tcp_stack/record_draw.txt');

x = X(:, 1);
y = X(:, 2);

plot(x, y);

set(get(gca, 'Title'), 'String', 'Time -- Cwnd');
set(get(gca, 'XLabel'), 'String', 'time (ms)');
set(get(gca, 'YLabel'), 'String', 'cwnd (bytes)');