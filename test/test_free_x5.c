#include "trasher.h"

int main() {
  // Test multiple free
  pool_status();
  free_pool_all();
  pool_status();
  free_pool_all();
  pool_status();
  free_pool_all();
  mem(100);
  pool_status();
  free_pool_all();
  pool_status();
}
