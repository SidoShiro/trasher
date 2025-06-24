#include "trasher.h"

// Power Printing macro to make pretty tests
#ifdef SUPPRESS_PRINT
#define POOL_STATUS() ((void)0)
#else
#define POOL_STATUS() pool_status()
#endif

int main() {
  // Test multiple free
  POOL_STATUS();
  free_pool_all();
  POOL_STATUS();
  free_pool_all();
  POOL_STATUS();
  free_pool_all();
  mem(100);
  POOL_STATUS();
  free_pool_all();
  POOL_STATUS();
}
