CC = gcc
CFLAGS = -O2 -Wall -I../src 

objects = hello echoservert badcnt goodcnt psum-mutex psum-array psum-local race norace

all: $(objects) parallel_sum_mutex parallel_sum_array parallel_sum_local
.PHONY: all 

$(objects): %: %.c
	$(CC) -o $@ $^ ../src/csapp.o $(CFLAGS)
ps1:parallel_sum_mutex.c
	$(CC) -o ps1 $^
ps2:parallel_sum_array.c
	$(CC) -o ps2 $^
ps3:parallel_sum_local.c
	$(CC) -o ps3 $^