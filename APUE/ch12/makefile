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

objects = 12.8.0 12.8.1 12.8.2 12.8.3 12.6.1
all: $(objects) 
.PHONY: all 

$(objects): %: %.c
	$(CC) -o $@ $^  $(HEADDIR) $(LIBDIR) $(LDLIBS) $(WFLAG)
