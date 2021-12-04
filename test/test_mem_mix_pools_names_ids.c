#include <stdio.h>
#include "trasher.h"

int main() {
  mem_id(50, 1);
  mem(100);
  mem_name(200, "Buffers");
  mem_id(300, 2);

  pool_status();

  return 0;
}
