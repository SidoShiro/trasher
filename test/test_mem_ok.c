#include <stdio.h>
#include "trasher.h"

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
  printf("%d %s", root->value, suffix);
  display_btree(root->left, " ");
  display_btree(root->right, " \n");
}

int main() {

  mem_id(50, 1);

  printf("Test mem stupid usage\n");

  // You should get the result of mem to do something with it
  mem(10);
  mem(20);
  mem(30);
  mem_name(50, "Check_Mate");

  mem_id(100, 1);

  pool_status();

  printf("Test mem normal usage\n");

  printf("Small Tree\n");
  struct btree *root_small = init_btree(30, NULL, NULL);
  display_btree(root_small, "\n");
  
  printf("Big Tree Init");
  struct btree *root = init_btree(0, init_btree(2, NULL, NULL), init_btree(42, NULL, init_btree(67, NULL, NULL)));

  printf("Print B Tree\n");
  display_btree(root, "\n");

  printf("Check pool\n");
  pool_status();

  printf("Free btrees pool\n");
  free_name("btrees");
  pool_status();
  printf("Create new pools, id=10, then with name=arguments\n");

  mem_id(16, 10);
  mem_name(64, "arguments");

  printf("Re-use free pool 'btrees', with a new tree:\n");
  struct btree *other_root = init_btree(5, init_btree(2, NULL, NULL), init_btree(6, NULL, NULL));
  display_btree(other_root, "\n");

  printf("Check pool\n");
  pool_status();

  printf("Free all\n");

  free_pool_all(); // Should close all pool functions for memory safety

  printf("End Test\n");
}
