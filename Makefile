# MakeFile to produce trasher lib
# Linux

CC = gcc
LIB = libtrasher.a
SRC = trasher/trasher.c

# -static
CFLAGS = -Wall
LDFLAGS = -L. -I. libtrasher.a 

TEST_SRC = test/main.c
TEST_BIN = bin_test

.PHONY: all

all: lib

#	ar -rcs ${LIB} trasher.o



lib:
	${CC} -c ${SRC} -o trasher.o
	ar -rc ${LIB} trasher.o
	rm trasher.o

test: lib
	cp trasher/trasher.h test/
	$(CC) $(CFLAGS) ${TEST_SRC} -o ${TEST_BIN} $(LDFLAGS)
	./${TEST_BIN}

clean:
	rm -f ${LIB} trasher.o ${TEST_BIN}

