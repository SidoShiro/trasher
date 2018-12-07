# MakeFile to produce trasher lib
# Linux

CC = gcc
LIB = trasher.a
SRC = trasher/trasher.c

.PHONY: all

all: lib

lib:
        ${CC} -c {SRC} -o trasher.o
        ar -rcs ${LIB} trasher.o
