/* Trasher Pool */
#include "trasher.h"

/* Global head pool memory manager */
struct pool_manager *manager;

void init_pool_manager()
{
  manager = malloc(sizeof(struct pool_manager));
  manager->hard = NULL;
  manager->soft = NULL;
}

struct pool_manager *get_pool()
{
  return manager;
}

static void add_to(struct mem_block *head, struct mem_block *newBlock)
{
  while (head->next != NULL)
    head = head->next;
  head->next = newBlock;
}

static void mem(struct mem_block *newBlock, enum mem_type type)
{
  switch (type)
  {
    case SOFT:
      if (get_pool()->soft)
        add_to(get_pool()->soft, newBlock);
      else
        get_pool()->soft = newBlock;
      break;
    case HARD:
      if (get_pool()->hard)
        add_to(get_pool()->hard, newBlock);
      else
        get_pool()->hard = newBlock;
      break;
    default:
      break;
  }
}

void *mem_s(size_t size)
{
  struct mem_block *newBlock = malloc(sizeof(struct mem_block));
  newBlock->data = malloc(size);
  newBlock->next = NULL;
  mem(newBlock, SOFT);
  return newBlock->data;
}

void *mem_h(size_t size)
{
  struct mem_block *newBlock = malloc(sizeof(struct mem_block));
  newBlock->data = malloc(size);
  newBlock->next = NULL;
  mem(newBlock, HARD);
  return newBlock->data;
}

static void rm_list_block(struct mem_block *head)
{
  if (head == NULL)
    return;
  struct mem_block *next = head->next;
  while (next)
  {
    free(head->data);
    free(head);
    head = next;
    next = next->next;
  }
  if (head != NULL)
  {
    free(head->data);
    free(head);
  }
}

void rm(enum mem_type type)
{
  switch (type)
  {
    case SOFT:
      rm_list_block(get_pool()->soft);
      get_pool()->soft = NULL;
      break;
    case HARD:
      rm_list_block(get_pool()->hard);
      get_pool()->hard = NULL;
      break;
    case ALL:
      rm_list_block(get_pool()->soft);
      rm_list_block(get_pool()->hard);
      free(get_pool());
      manager = NULL;
  }
}
