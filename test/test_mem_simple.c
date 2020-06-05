#include <stdio.h>
#include "trasher.h"

int main() {
  size_t buffer_size = 10;
  char* buffer = mem(buffer_size);
  buffer[0] = 10;

  mem(20);

  mem(30);

  mem(40);

  mem(50);

  pool_status();

  free_pool_all();
  return 0;
}
