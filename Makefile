# MakeFile to produce libtrasher lib
# Linux

CC = gcc
LIB = libtrasher.a
SRC = libtrasher/trasher.c

# -static
CFLAGS = -Wall -Wextra
LDFLAGS = -L. -ltrasher
LDFLAGS_CUNIT = -L. -ltrasher -lcunit
CDEBUGFLAGS = -O0 -Wall -Wextra -g
CDEBUGFLAGSFSANITIZE = -O0 -Wall -Wextra -g -fsanitize=address

TEST_SRC_KO = test/test_mem_ko.c
TEST_SRC_OK = test/test_mem_ok.c
TEST_SRC_SIMPLE = test/test_mem_simple.c
TEST_SRC_MEM = test/test_mem_valgrind.c
TEST_SRC_MIX = test/test_mem_mix_pools_names_ids.c
TEST_SRC_UNIT = test/test_mem_unit_test.c
TEST_SRC_FREE_5X = test/test_free_x5.c
TEST_SRC_PERF = test/test_speed_perf.c
TEST_BIN_OK = bin_test_mem_ok
TEST_BIN_KO = bin_test_mem_ko
TEST_BIN_SIMPLE = bin_test_mem_simple
TEST_BIN_MEM = bin_test_mem_valgrind
TEST_BIN_MIX = bin_test_mem_mix
TEST_BIN_UNIT = bin_test_unit
TEST_BIN_FREE_5X = bin_test_free_5x
TEST_BIN_PERF = bin_test_perf

.PHONY: all
.PHONY: clean
.PHONY: lib

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

lib_debug_fsanitize: clean 
	${CC} -c ${SRC} ${CDEBUGFLAGSFSANITIZE} -o trasher.o
	ar -rc ${LIB} trasher.o
	rm trasher.o

lib_debug_valgrind: clean
	${CC} -c ${SRC} ${CDEBUGFLAGS} -o trasher.o
	ar -rc ${LIB} trasher.o
	rm trasher.o

test: lib_debug test_ok test_simple test_mix_pools_names_ids test_memcheck_ok_fsanitize test_memcheck_ok_valgrind test_unit clean

test_all_except_cunit_tests: lib_debug test_ok test_simple test_mix_pools_names_ids test_memcheck_ok_fsanitize test_memcheck_ok_valgrind clean


test_ko: lib_debug
	cp libtrasher/trasher.h test/
	$(CC) $(CDEBUGFLAGS) ${TEST_SRC_KO} -o ${TEST_BIN_KO} $(LDFLAGS)
	./${TEST_BIN_KO}

test_ok: lib_debug
	cp libtrasher/trasher.h test/
	$(CC) $(CDEBUGFLAGS) ${TEST_SRC_OK} -o ${TEST_BIN_OK} $(LDFLAGS)
	./${TEST_BIN_OK}

test_simple: lib_debug
	cp libtrasher/trasher.h test/
	$(CC) $(CDEBUGFLAGS) ${TEST_SRC_SIMPLE} -o ${TEST_BIN_SIMPLE} $(LDFLAGS)
	./${TEST_BIN_SIMPLE}

test_mix_pools_names_ids: lib_debug
	cp libtrasher/trasher.h test/
	$(CC) $(CDEBUGFLAGS) ${TEST_SRC_MIX} -o ${TEST_BIN_MIX} $(LDFLAGS)
	./${TEST_BIN_MIX}

test_memcheck_ok_fsanitize: lib_debug_fsanitize
	cp libtrasher/trasher.h test/
	$(CC) $(CDEBUGFLAGSFSANITIZE) ${TEST_SRC_MEM} -o ${TEST_BIN_MEM} $(LDFLAGS)
	./${TEST_BIN_MEM}

test_memcheck_ok_valgrind: lib_debug_valgrind
	cp libtrasher/trasher.h test/
	$(CC) $(CDEBUGFLAGS) ${TEST_SRC_MEM} -o ${TEST_BIN_MEM} $(LDFLAGS)
	valgrind -s --track-origins=yes --leak-check=full ./${TEST_BIN_MEM}

# Requires CUNIT lib installed
test_unit: lib_debug
	cp libtrasher/trasher.h test/
	$(CC) $(CDEBUGFLAGS) $(TEST_SRC_UNIT) -o $(TEST_BIN_UNIT) $(LDFLAGS_CUNIT)
	./$(TEST_BIN_UNIT)

test_unit_verbose: lib_debug
	cp libtrasher/trasher.h test/
	$(CC) $(CDEBUGFLAGS) -D SHOW_POOLS_STATUS $(TEST_SRC_UNIT) -o $(TEST_BIN_UNIT) $(LDFLAGS)
	./$(TEST_BIN_UNIT)

test_free_5x: lib_debug
	cp libtrasher/trasher.h test/
	$(CC) $(CDEBUGFLAGS) $(TEST_SRC_FREE_5X) -o $(TEST_BIN_FREE_5X) $(LDFLAGS)
	./${TEST_BIN_FREE_5X}

test_perf: lib
	cp libtrasher/trasher.h test/
	$(CC) $(CFLAGS) -g $(TEST_SRC_PERF) -o $(TEST_BIN_PERF) $(LDFLAGS)
	./${TEST_BIN_PERF}

clean:
	rm -f ${LIB} trasher.o ${TEST_BIN_KO} ${TEST_BIN_OK} ${TEST_BIN_SIMPLE} ${TEST_BIN_UNIT} ${TEST_BIN_MIX} ${TEST_BIN_FREE_5X} ${TEST_BIN_MEM}

