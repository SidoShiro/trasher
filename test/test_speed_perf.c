#include <stdio.h>
#include <time.h>

#include "trasher.h"

// Power Printing macro to make pretty tests
#ifdef SUPPRESS_PRINT
#define PRINT(fmt, ...) ((void)0)
#else
#define PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#endif

#define BLOCKS 2000
#define BLOCK_SIZE 1024

void test_trasher() {
  unsigned sizes_len = BLOCKS;

  for (unsigned i = 0; i < sizes_len; i++) {
    mem(BLOCK_SIZE);
  }

  free_pool_all();
}

void test_malloc() {
  unsigned sizes_len = BLOCKS;
  void* ptrs[sizes_len];

  for (unsigned i = 0; i < sizes_len; i++) {
    ptrs[i] = malloc(BLOCK_SIZE);
  }

  for (unsigned i = 0; i < sizes_len; i++) {
    free(ptrs[i]);
  }
}

double speed_tester(const char *lib_tested, void (*function_to_test)()) {
  clock_t start;
  clock_t end;
  double cpu_time_used_sum = 0;
  double cpu_time_used_average = 0;

  // Run test 1000 times to have average estimate and less border effects
  double i = 1;
  for (; i < 1000; i++) {
    start = clock();
    /* Execute timed function */
    function_to_test();
    end = clock();
    cpu_time_used_sum += ((double) (end - start)) / CLOCKS_PER_SEC;
  }
  cpu_time_used_average = cpu_time_used_sum / i;
  PRINT("Average time for %10s lib : %f\n", lib_tested, cpu_time_used_average);
  return cpu_time_used_average;
}

int main() {
  PRINT("Test timing and diff with malloc basic\n");
  double time_avg_malloc = speed_tester("malloc", test_malloc);
  double time_avg_trasher = speed_tester("trasher", test_trasher);
  double pourcentage = (time_avg_malloc / time_avg_trasher) * 100;
  PRINT("Difference in %% : %f %% less efficient than malloc\n", pourcentage);
  PRINT("End Test\n");
}
