#include <stdio.h>
#include "trasher.h"

int main() {

  unsigned sizes_len = 0;
  const int sizes[] = { 16, 16, 32, 64, 128, 512, 1024, 1024, 1024, 1024, 2048, 2048, 16384, 20000, 65536, 0 };
  while (sizes[sizes_len++] != 0) {}

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

  printf("End Test\n");
}
