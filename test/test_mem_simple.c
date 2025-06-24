#include <stdio.h>
#include "trasher.h"

// Power Printing macro to make pretty tests
#ifdef SUPPRESS_PRINT
#define POOL_STATUS() ((void)0)
#else
#define POOL_STATUS() pool_status()
#endif

int main() {
  size_t buffer_size = 10;
  char* buffer = mem(buffer_size);
  buffer[0] = 10;

  mem(20);

  mem(30);

  mem(40);

  mem(50);

  POOL_STATUS();

  free_pool_all();
  return 0;
}
