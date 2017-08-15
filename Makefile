#需要把当前include文件夹加入路径
CC      = gcc
INCLUDE = include
cflags	=  -std=gnu11 -fdiagnostics-color=auto -Wall -g
SRC		= example/test_mock.c src/core.c src/io.c src/list.c src/mock.c src/assert.c
test:$(SRC)
	$(CC) $(cflags)  -I $(INCLUDE) $(SRC) -o test.bin