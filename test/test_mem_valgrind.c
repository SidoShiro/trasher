#include <stdio.h>
#include "trasher.h"

// Power Printing macro to make pretty tests
#ifdef SUPPRESS_PRINT
#define PRINT(fmt, ...) ((void)0)
#define POOL_STATUS() ((void)0)
#else
#define PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define POOL_STATUS() pool_status()
#endif

int main() {
  unsigned sizes_len = 0;
  const int sizes[] = { 1, 16, 32, 64, 128, 512, 1024, 1024, 1024, 1024, 2048, 2048, 16384, 20000, 65536, 0 };
  while (sizes[sizes_len++] != 0) {}

  mem(42);

  POOL_STATUS();

  PRINT("Generate Mo of data into multiple pools\n");

  const char* names[] = { "CARS", "PLANES", "BIKES", "BOATS", NULL };
  
  for (unsigned i = 0; names[i] != NULL; i++) {
    for (unsigned j = 0; j < sizes_len; j++) {
      mem_name(sizes[j], names[i]);
    }
  }

  PRINT("Filled pools...\n");

  POOL_STATUS();

  PRINT("Free all\n");
  free_pool_all(); // Should close all pool functions for memory safety
  PRINT("Second free should do no effects\n");
  free_pool_all(); // Should close all pool functions for memory safety

  PRINT("Generate Mo of data into multiple pools... 2nd phase\n");

  const char* names_2nd[] = { "HOMES", "COMPANIES", "STOCKS", "TABLES", "PENS", "SYSTEMS", "WEBS", NULL };
  const int sizes_2nd[] = { 16, 1024, 1024, 2048, 2048, 16384, 200, 200, 200, 200, 200, 636, 0 };
  unsigned sizes_2nd_len = 0;
  while (sizes_2nd[sizes_2nd_len++] != 0) {}

  mem_name(1, "HOMES");
  mem_name(2, "HOMES");
  mem_name(84, "HOMES");

  for (unsigned i = 1; names_2nd[i] != NULL; i++) {
    for (unsigned j = 0; j < sizes_2nd_len; j++) {
      mem_name(sizes_2nd[j], names_2nd[i]);
    }
  }

  free_pool();

  POOL_STATUS();

  free_id(5);
  free_id(5);

  free_name("HOMES");

  free_pool_all(); // Should close all pool functions for memory safety

  PRINT("End Test\n");
}
