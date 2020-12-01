INCLUDE = include
CFLAGS = -g
CC = gcc


all: lookup

lookup: main.c one_bit_prefix_tree.o tool.o
	$(CC) $(CFLAGS) -I $(INCLUDE) -o lookup main.c one_bit_prefix_tree.o tool.o

one_bit_prefix_tree.o: one_bit_prefix_tree.c
	$(CC) $(CFLAGS) -I $(INCLUDE) -c one_bit_prefix_tree.c

tool.o: tool.c 
	$(CC) $(CFLAGS) -I $(INCLUDE) -c tool.c 

clean:
	rm *.o lookup