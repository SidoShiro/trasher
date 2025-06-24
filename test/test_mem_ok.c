#include <stdio.h>
#include "trasher.h"

// Power Printing macro to make pretty tests
#ifdef SUPPRESS_PRINT
#define PRINT(fmt, ...) ((void)0)
#define POOL_STATUS() ((void)0)
#else
#define PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define POOL_STATUS() pool_status()
#endif

struct btree {
  int value;
  struct btree *left;
  struct btree *right;
};

struct btree *init_btree(int value, struct btree *left, struct btree *right) {
  struct btree *node = mem_name(sizeof(struct btree), "btrees");
  node->value = value;
  node->left = left;
  node->right = right;
  return node;
}

void display_btree(struct btree *root, char* suffix) {
  if (root == NULL)
    return;
  PRINT("%d %s", root->value, suffix);
  display_btree(root->left, " ");
  display_btree(root->right, " \n");
}

int main() {

  mem_id(50, 1);

  PRINT("Test mem stupid usage\n");

  // You should get the result of mem to do something with it
  mem(10);
  mem(20);
  mem(30);
  mem_name(50, "Check_Mate");

  mem_id(100, 1);

  POOL_STATUS();

  PRINT("Test mem normal usage\n");

  PRINT("Small Tree\n");
  struct btree *root_small = init_btree(30, NULL, NULL);
  display_btree(root_small, "\n");
  
  PRINT("Big Tree Init");
  struct btree *root = init_btree(0, init_btree(2, NULL, NULL), init_btree(42, NULL, init_btree(67, NULL, NULL)));

  PRINT("Print B Tree\n");
  display_btree(root, "\n");

  PRINT("Check pool\n");
  POOL_STATUS();

  PRINT("Free btrees pool\n");
  free_name("btrees");
  POOL_STATUS();
  PRINT("Create new pools, id=10, then with name=arguments\n");

  mem_id(16, 10);
  mem_name(64, "arguments");

  PRINT("Re-use free pool 'btrees', with a new tree:\n");
  struct btree *other_root = init_btree(5, init_btree(2, NULL, NULL), init_btree(6, NULL, NULL));
  display_btree(other_root, "\n");

  PRINT("Check pool\n");
  POOL_STATUS();

  PRINT("Free all\n");

  free_pool_all(); // Should close all pool functions for memory safety

  PRINT("End Test\n");
}
