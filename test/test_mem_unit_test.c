#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "trasher.h"

// Init Suite
int init_unit_test_suite() {
  // Open, init,vars or file used by tests
  return 0;
}

int clean_suite() {
  free_pool_all();
  // Close stuff opened in init_unit_test_suite
  return 0;
}

void testFreePoolsWhenEmpty() {
  pool_status();
  free_pool_all();
  pool_status();
  free_pool_all();
}

void testPoolGetNameTooHighValue() {
  free_pool_all();
  mem_name(128, "Buffer");
  pool_status();
  char *n = pool_give_name_from_id(10);
  CU_ASSERT(n == NULL);
  char *n2 = pool_give_name_from_id(2);
  CU_ASSERT(n2 == NULL);
  char *n3 = pool_give_name_from_id(1);
  CU_ASSERT(n3 == NULL);
}

void testPoolGetName() {
  free_pool_all();
  mem_name(128, "Buffer");
  pool_status();
  char *name = pool_give_name_from_id(1);
  CU_ASSERT(name != NULL);
  if (name == NULL)
    return;
  CU_ASSERT(strcmp(name, "Buffer") == 0);
  mem_name(512, "firefox");
  mem_name(128, "Dart");
  mem_name(128, "Xenon");
  pool_status();
  char *n3 = pool_give_name_from_id(3);
  if (n3 == NULL) {
    CU_ASSERT(n3 != NULL);
    return;
  }
  CU_ASSERT(strcmp(n3, "Dart") == 0);
  char *n4 = pool_give_name_from_id(4);
  if (n4 == NULL) {
    CU_ASSERT(n4 != NULL);
    return;
  }
  CU_ASSERT(strcmp(n4, "Xenon") == 0);
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
  if ((NULL == CU_add_test(pSuite, "free pools when empty", testFreePoolsWhenEmpty)) ||
      (NULL == CU_add_test(pSuite, "get pool name test", testPoolGetName)) ||
      (NULL == CU_add_test(pSuite, "get pool name other wrong value", testPoolGetNameTooHighValue))
    ) {
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
