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

objects = 1-3 1-4 1-5 1-6 1-7 1-8 1-9 1-10
all: $(objects)
.PHONY: all 

$(objects): %: %.c
	$(CC) -o $@ $^  $(HEADDIR) $(LIBDIR) $(LDLIBS)