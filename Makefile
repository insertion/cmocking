#需要把当前include文件夹加入路径
CC      = gcc
INCLUDE = include
cflags	= -Wall -std=gnu11
SRC		= example/run_test.c src/core.c src/io.c
test:$(SRC)
	$(CC) $(cflags)  -I $(INCLUDE) $(SRC) -o test.bin