# MakeFile to produce libtrasher lib
# Linux

CC = gcc
LIB = libtrasher.a
SRC = libtrasher/trasher.c

# -static
CFLAGS = -Wall -Wextra
LDFLAGS = -L. -ltrasher
CDEBUGFLAGS = -Wall -Wextra -g -fsanitize=address

TEST_SRC_KO = test/test_mem_ko.c
TEST_SRC_OK = test/test_mem_ok.c
TEST_SRC_SIMPLE = test/test_mem_simple.c
TEST_SRC_MEM = test/test_mem_valgrind.c
TEST_BIN_OK = bin_test_mem_ok
TEST_BIN_KO = bin_test_mem_ko
TEST_BIN_SIMPLE = bin_test_mem_simple
TEST_BIN_MEM = bin_test_mem_valgrind

.PHONY: all


all: lib

#	ar -rcs ${LIB} libtrasher.o

lib: clean
	${CC} -c ${SRC} ${CFLAGS} -o trasher.o
	ar -rc ${LIB} trasher.o
	rm trasher.o


lib_debug: clean 
	${CC} -c ${SRC} ${CDEBUGFLAGS} -o trasher.o
	ar -rc ${LIB} trasher.o
	rm trasher.o


test: lib_debug test_ok test_simple test_memcheck_ok

test_ko:
	cp libtrasher/trasher.h test/
	$(CC) $(CDEBUGFLAGS) -g ${TEST_SRC_KO} -o ${TEST_BIN_KO} $(LDFLAGS)
	./${TEST_BIN_KO}

test_ok:
	cp libtrasher/trasher.h test/
	$(CC) $(CDEBUGFLAGS) -g ${TEST_SRC_OK} -o ${TEST_BIN_OK} $(LDFLAGS)
	./${TEST_BIN_OK}

test_simple:
	cp libtrasher/trasher.h test/
	$(CC) $(CDEBUGFLAGS) -g ${TEST_SRC_SIMPLE} -o ${TEST_BIN_SIMPLE} $(LDFLAGS)
	./${TEST_BIN_SIMPLE}

test_memcheck_ok: lib
	cp libtrasher/trasher.h test/
	$(CC) $(CFLAGS) -g ${TEST_SRC_MEM} -o ${TEST_BIN_MEM} $(LDFLAGS)
	valgrind --track-origins=yes ./${TEST_BIN_MEM}

clean:
	rm -f ${LIB} trasher.o ${TEST_BIN_KO} ${TEST_BIN_OK} ${TEST_BIN_SIMPLE}

