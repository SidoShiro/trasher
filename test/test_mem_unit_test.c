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
  free_pool_all();
}


void testPoolGiveNumberBlocks() {
  free_pool_all();
  pool_status();
  ssize_t n = pool_give_number_blocks(0);
  CU_ASSERT(n == -1);
  mem_id(16, 0);
  pool_status();
  n = pool_give_number_blocks(0);
  CU_ASSERT(n == 1);
  mem_id(16, 0);
  n = pool_give_number_blocks(0);
  CU_ASSERT(n == 2);
  mem_id(20, 0);
  mem_id(20, 0);
  mem_id(20, 0);
  n = pool_give_number_blocks(0);
  CU_ASSERT(n == 5);
  n = pool_give_number_blocks(1);
  CU_ASSERT(n == -2);
  mem_id(46, 10);
  n = pool_give_number_blocks(10);
  CU_ASSERT(n == 1);
  mem_id(90, 11);
  mem_id(120, 11);
  n = pool_give_number_blocks(11);
  CU_ASSERT(n == 2);
  int j = 100000;
  for (int i = 0; i != j; i++)
    mem_id(8, 100);
  n = pool_give_number_blocks(100);
  CU_ASSERT(n == j);
  // pool_status(); show 100 000 8 so useless
  free_pool_all();
}

int main() {
  // CU lib checks
  if (CUE_SUCCESS != CU_initialize_registry()) {
    return CU_get_error();
  }

  // Suite for pool manager creation, deletion
  CU_pSuite suitePoolManagerCreDel = CU_add_suite("Pool Manager", init_unit_test_suite, clean_suite);
  // Suite for simple mem add behaviour
  CU_pSuite suiteMemAdd = CU_add_suite("Mem add behaviours", init_unit_test_suite, clean_suite);
  // Suite about freeing pools
  CU_pSuite suiteFreePools = CU_add_suite("Free pools behaviours", init_unit_test_suite, clean_suite);
  // Suite for pool names
  CU_pSuite suitePoolNames = CU_add_suite("Pool Names", init_unit_test_suite, clean_suite);
  // Suite for pool block numbers
  CU_pSuite suitePoolBlocks = CU_add_suite("Pool blocks numbers", init_unit_test_suite, clean_suite);

  if (NULL == suitePoolManagerCreDel ||
      NULL == suitePoolBlocks ||
      NULL == suitePoolNames ||
      NULL == suiteFreePools ||
      NULL == suiteMemAdd) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Add tests to the suite
  if ((NULL == CU_add_test(suiteFreePools, "free pools when empty", testFreePoolsWhenEmpty)) ||
      (NULL == CU_add_test(suitePoolNames, "get pool name test", testPoolGetName)) ||
      (NULL == CU_add_test(suitePoolNames, "get pool name other wrong value", testPoolGetNameTooHighValue)) ||
      (NULL == CU_add_test(suitePoolBlocks, "give number of blocks in a pool", testPoolGiveNumberBlocks))
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
