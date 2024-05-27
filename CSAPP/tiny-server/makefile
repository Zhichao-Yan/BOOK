CC = gcc
CFLAGS = -g -Wall -Wno-format-overflow -I$(INCLUDE)
INCLUDE := ../code
CGI_DIR := cgi-bin
CGI_FILES := $(wildcard $(CGI_DIR)/*.c)
CGI_EXE := $(patsubst %.c,%,$(CGI_FILES))

all: http $(CGI_EXE) main

http: http.c $(INCLUDE)/csapp.c
	$(CC) $(CFLAGS) $^ -o $@ 

$(CGI_EXE): %: %.c $(INCLUDE)/csapp.c
	$(CC) $(CFLAGS) $^ -o $@
main: main.c
	$(CC) $(CFLAGS) $^ -o $@ 

clean:
	rm -f http $(CGI_EXE)

.PHONY: all clean