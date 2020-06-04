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
  printf("Test mem normal usage\n");

  printf("Small Tree\n");
  struct btree *root_small = init_btree(30, NULL, NULL);
  display_btree(root_small, "\n");
  
  printf("Big Tree Init");
  struct btree *root = init_btree(0, init_btree(2, NULL, NULL), init_btree(42, NULL, init_btree(67, NULL, NULL)));

  printf("Print B Tree\n");
  display_btree(root, "\n");


  printf("Check pool");
  pool_status();

  
  free_name("btrees");

  pool_status();

  printf("Free all\n");
  free_pool_all();

  pool_status();

  printf("End Test\n");
}
