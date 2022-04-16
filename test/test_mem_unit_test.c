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

void print_pools();

#ifdef SHOW_POOLS_STATUS
void print_pools() {
  pool_status();
}
#endif
#ifndef SHOW_POOLS_STATUS
void print_pools() {}
#endif

void testFreePoolsWhenEmpty() {
  print_pools();
  free_pool_all();
  print_pools();
  free_pool_all();
}

void testPoolGetNameTooHighValue() {
  free_pool_all();
  mem_name(128, "Buffer");
  print_pools();
  char *n = pool_give_name_from_id(10);
  CU_ASSERT_PTR_NULL(n);
  char *n2 = pool_give_name_from_id(2);
  CU_ASSERT_PTR_NULL(n2);
  char *n3 = pool_give_name_from_id(0);
  CU_ASSERT_PTR_NULL(n3);
  char *buffer = pool_give_name_from_id(1);
  CU_ASSERT_PTR_NOT_NULL(buffer);
}

void testPoolGetName() {
  free_pool_all();
  mem_name(128, "Buffer");
  // First Pool is Null name
  char *default_name_null = pool_give_name_from_id(0);
  CU_ASSERT_PTR_NULL(default_name_null);
  print_pools();
  char *name = pool_give_name_from_id(1);
  CU_ASSERT_PTR_NOT_NULL(name);
  if (name == NULL)
    return;
  CU_ASSERT_STRING_EQUAL(name, "Buffer");
  // Get pool name of nonexistent pool => should return NULL
  char *n2 = pool_give_name_from_id(2);
  if (n2 == NULL) {
    CU_ASSERT_PTR_NULL(n2);
    return;
  }
  CU_ASSERT_STRING_EQUAL(n2, "firefox");
  mem_name(512, "firefox");
  n2 = pool_give_name_from_id(2);
  if (n2 == NULL) {
    CU_ASSERT_PTR_NOT_NULL(n2);
    return;
  }
  CU_ASSERT_STRING_EQUAL(n2, "firefox");
  mem_name(128, "Dart");
  mem_name(128, "Xenon");
  print_pools();
  char *n3 = pool_give_name_from_id(3);
  if (n3 == NULL) {
    CU_ASSERT_PTR_NOT_NULL(n3);
    return;
  }
  CU_ASSERT_STRING_EQUAL(n3, "Dart");
  char *n4 = pool_give_name_from_id(4);
  if (n4 == NULL) {
    CU_ASSERT_PTR_NOT_NULL(n4);
    return;
  }
  CU_ASSERT_STRING_EQUAL(n4, "Xenon");
  mem_name(128, "Xenon");
  mem_name(128, "Xenon");
  size_t id_4 = pool_give_id_from_name("Xenon");
  // Xenon has 3 block of 128 bytes
  CU_ASSERT_EQUAL(pool_give_number_blocks(id_4), 3);
  free_pool_all();
}

void testPoolNameBehaviour() {
  free_pool_all();
  mem_name(1, "HOMES");
  mem_name(2, "HOMES");
  mem_name(84, "HOMES");
  CU_ASSERT_NOT_EQUAL(pool_give_id_from_name("HOMES"), 0);
  // pool_status_debug();
  CU_ASSERT_EQUAL(pool_give_id_from_name("HOMES"), 1);
  CU_ASSERT_EQUAL(pool_give_number_blocks(0), 0);
  CU_ASSERT_EQUAL(pool_give_number_blocks(1), 3);
  // pool_status_debug();
  free_pool_all();
}

void testPoolNameBehaviourWithId() {
  mem(10);
  mem_id(10, 1);
  mem_name(20, "X");
  mem_name(30, "A");
  mem_id(30, 2);
  mem_id(8, 8);
  mem_name(96, "B");
  pool_status_debug();
}

void testPoolGiveNumberBlocks() {
  free_pool_all();
  print_pools();
  ssize_t n = pool_give_number_blocks(0);
  CU_ASSERT(n == -1);
  mem_id(16, 0);
  print_pools();
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
  int j = 18234;
  for (int i = 0; i != j; i++)
    mem_id(8, 100);
  n = pool_give_number_blocks(100);
  CU_ASSERT(n == j);
  // print_pools(); show 100 000 8 so useless
  free_pool_all();
}

void testPoolManagerInit() {
  struct pool_manager *pm = get_pool_manager(0);
  CU_ASSERT(pm != NULL);
  CU_ASSERT(0 == pm->pools_nb);
  CU_ASSERT(NULL == pm->pools);
  CU_ASSERT(NULL == pm->names);
}

void testPoolManagerAddCheck() {
  struct pool_manager *pm = get_pool_manager(0);
  mem(8);
  CU_ASSERT(1 == pm->pools_nb);
  CU_ASSERT(NULL != pm->names);
  CU_ASSERT(NULL != pm->pools);
  if (pm->names != NULL)
    CU_ASSERT(pm->names[0] == NULL);
  if (pm->pools) {
    struct mem_block *b = pm->pools[0];
    CU_ASSERT(8 == b->data_size);
    CU_ASSERT(NULL == b->next);
  }
  mem(32);
  struct pool_manager *pm_2 = get_pool_manager(0);
  CU_ASSERT(pm == pm_2);
  
  // pool name
  mem_name(128, "Jojo");
  CU_ASSERT(0 == strcmp("Jojo", pm->names[1]));
  
  // pool id
  mem_id(42, 10);
  print_pools();
  CU_ASSERT(42 == pm->pools[10]->data_size);
}

void testPoolManagerFree() {
  struct pool_manager *pm = get_pool_manager(0);
  mem(28); 
  print_pools();
  free_pool_all();
  void *p = malloc(sizeof(struct pool_manager));
  struct pool_manager *pm_2 = get_pool_manager(0);
  CU_ASSERT(pm != pm_2);
  print_pools();
  free(p);
}

void testPoolManagerMultipleFree() {
  mem(24);
  mem_name(16, "GOOD");
  mem_id(42, 42);
  struct pool_manager *pm = get_pool_manager(0);
  CU_ASSERT_EQUAL(pm->pools_nb, 43);
  free_pool();
  free_id(20);
  mem_id(20, 20);
  CU_ASSERT_EQUAL(pool_give_number_blocks(20), 1);
  free_name("GOOD");
  mem_name(32, "GOOD");
  mem_id(64, 1);
  CU_ASSERT_EQUAL(pool_give_id_from_name("GOOD"), 1);
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
      (NULL == CU_add_test(suitePoolNames, "special pool name behaviour", testPoolNameBehaviour)) ||
      (NULL == CU_add_test(suitePoolNames, "pool name behaviour with id", testPoolNameBehaviourWithId)) ||
      (NULL == CU_add_test(suitePoolNames, "get pool name other wrong value", testPoolGetNameTooHighValue)) ||
      (NULL == CU_add_test(suitePoolBlocks, "give number of blocks in a pool", testPoolGiveNumberBlocks)) ||
      (NULL == CU_add_test(suitePoolManagerCreDel, "initialization of pool manager", testPoolManagerInit)) ||
      (NULL == CU_add_test(suitePoolManagerCreDel, "simple add and checks", testPoolManagerAddCheck)) ||
      (NULL == CU_add_test(suitePoolManagerCreDel, "free pool manager", testPoolManagerFree)) ||
      (NULL == CU_add_test(suitePoolManagerCreDel, "multiple free pool manager", testPoolManagerMultipleFree))
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
