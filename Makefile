# MakeFile to produce trasher lib
# Linux

CC = gcc
LIB = trasher.a
SRC = trasher/trasher.c

.PHONY: all

all: lib

lib: ${SRC}
    ar -rcs ${LIB} trasher.o
