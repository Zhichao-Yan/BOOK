CC = gcc
CFLAGS = -g -Wall -Wno-format-overflow -I$(INCLUDE)
INCLUDE := ../code
CGI_DIR := cgi-bin
CGI_FILES := $(wildcard $(CGI_DIR)/*.c)
CGI_EXE := $(patsubst %.c,%,$(CGI_FILES))

all: tiny $(CGI_EXE)

tiny: tiny.c $(INCLUDE)/csapp.c
	$(CC) $(CFLAGS) $^ -o $@ 

$(CGI_EXE): %: %.c $(INCLUDE)/csapp.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f tiny $(CGI_EXE)

.PHONY: all clean