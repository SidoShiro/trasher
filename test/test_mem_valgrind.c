#include <stdio.h>
#include "trasher.h"

int main() {

  unsigned sizes_len = 0;
  const int sizes[] = { 1, 16, 32, 64, 128, 512, 1024, 1024, 1024, 1024, 2048, 2048, 16384, 20000, 65536, 0 };
  while (sizes[sizes_len++] != 0) {}

  mem(42);

  pool_status();

  printf("Generate Mo of data into multiple pools\n");

  const char* names[] = { "CARS", "PLANES", "BIKES", "BOATS", NULL };
  
  for (unsigned i = 0; names[i] != NULL; i++) {
    for (unsigned j = 0; j < sizes_len; j++) {
      mem_name(sizes[j], names[i]);
    }
  }

  printf("Filled pools...\n");

  pool_status();

  printf("Free all\n");
  free_pool_all(); // Should close all pool functions for memory safety
  printf("Second free should do no effects\n");
  free_pool_all(); // Should close all pool functions for memory safety

  printf("Generate Mo of data into multiple pools... 2nd phase\n");

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

  pool_status();

  free_id(5);
  free_id(5);

  free_name("HOMES");

  free_pool_all(); // Should close all pool functions for memory safety

  printf("End Test\n");
}
