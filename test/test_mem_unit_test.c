#include <stdio.h>
#include <CUnit/Basic.h>
#include "trasher.h"

// Init Suite
int init_unit_test_suite() {
  // Open, init,vars or file used by tests
  return 0;
}

int clean_suite() {
  // Close stuff opened in init_unit_test_suite
  return 0;
}

void testSimple() {
  CU_ASSERT(2 == 2);
  CU_ASSERT(2 == 3);
  return;
}

int main() {

  CU_pSuite pSuite = NULL;

  if (CUE_SUCCESS != CU_initialize_registry()) {
    return CU_get_error();
  }

  pSuite = CU_add_suite("Unit tests", init_unit_test_suite, clean_suite);

  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Add tests to the suite
  if ((NULL == CU_add_test(pSuite, "simple test", testSimple))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Run tests
  // Basic interface
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return 0;
}
