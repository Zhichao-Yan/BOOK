SHELL=/bin/bash
CC = gcc
CXX = g++
LDLIBS = -lapue
# Extra flags to give to compilers when they are supposed to invoke the linker
LDFLAGS = 
# Extra flags to give to the C preprocessor
CPPFLAGS = 
# Extra flags to give to the C++ compiler
CXXFLAGS = 
# Extra flags to give to the C compiler
CFLAGS = 
# Search directory for Head
HEADDIR = -iquote ../apue.3e/include
# Search directory for Lib
LIBDIR =  -L../apue.3e/lib
WFLAG =-Wall

objects = 8-1 8-3 8-8 8-12 8-13 8-17 8-16
all: $(objects) 8-5-6
.PHONY: all 

$(objects): %: %.c
	$(CC) -o $@ $^  $(HEADDIR) $(LIBDIR) $(LDLIBS) $(WFLAG)

8-5-6:8-5.c 8-6.c
	$(CC) -o $@ $^ $(HEADDIR) $(LIBDIR) $(LDLIBS) $(WFLAG)