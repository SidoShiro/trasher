#include <stdio.h>
#include "trasher.h"

// Power Printing macro to make pretty tests
#ifdef SUPPRESS_PRINT
#define POOL_STATUS() ((void)0)
#else
#define POOL_STATUS() pool_status()
#endif


int main() {
  mem_id(50, 1);
  mem(100);
  mem_name(200, "Buffers");
  mem_id(300, 2);

  POOL_STATUS();

  return 0;
}
