# MakeFile to produce libtrasher lib
# Linux

CC = gcc
LIB = libtrasher.a
SRC = libtrasher/trasher.c

# -static
CFLAGS = -Wall
LDFLAGS = -L. -I. libtrasher.a 

TEST_SRC_KO = test/test_mem_ko.c
TEST_SRC_OK = test/test_mem_ok.c
TEST_BIN_OK = bin_test_mem_ko
TEST_BIN_KO = bin_test_mem_ok

.PHONY: all

all: lib

#	ar -rcs ${LIB} libtrasher.o



lib:
	${CC} -c ${SRC} -o trasher.o
	ar -rc ${LIB} trasher.o
	rm trasher.o

test_ko: lib
	cp libtrasher/trasher.h test/
	$(CC) $(CFLAGS) ${TEST_SRC_KO} -o ${TEST_BIN_KO} $(LDFLAGS)
	./${TEST_BIN_KO}

test_ok: lib
	cp libtrasher/trasher.h test/
	$(CC) $(CFLAGS) ${TEST_SRC_OK} -o ${TEST_BIN_OK} $(LDFLAGS)
	./${TEST_BIN_OK}

clean:
	rm -f ${LIB} trasher.o ${TEST_BIN_KO} ${TEST_BIN_OK}

