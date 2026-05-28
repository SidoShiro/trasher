#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "trasher.h"

void test_null_inputs() {
    printf("Testing NULL inputs... ");
    assert(mem_name(10, NULL) == NULL);
    assert(pool_give_id_from_name(NULL) == -1);
    assert(pool_rename(NULL, "test") == -1);
    assert(pool_rename("test", NULL) == -1);
    free_name(NULL); // Should not crash
    printf("OK\n");
}

void test_large_id() {
    printf("Testing large pool ID... ");
    void *ptr = mem_id(10, 1000);
    assert(ptr != NULL);
    assert(pool_give_number_blocks(1000) == 1);
    free_id(1000);
    assert(pool_give_number_blocks(1000) == 0);
    printf("OK\n");
}

void test_empty_pool_operations() {
    printf("Testing operations on empty pools... ");
    free_pool_all();
    free_pool(); // Should not crash
    free_id(0);  // Should not crash
    free_name("nonexistent"); // Should not crash
    assert(pool_give_id_from_name("nonexistent") == -1);
    assert(pool_give_number_blocks(0) == -1); // Manager is NULL after free_pool_all
    printf("OK\n");
}

int main() {
    test_null_inputs();
    test_large_id();
    test_empty_pool_operations();
    
    free_pool_all();
    printf("All robustness tests passed!\n");
    return 0;
}
