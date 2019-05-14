#ifndef TRASHER_H
#define TRASHER_H

/* steddef and stdlib needed to use malloc & cie */
#include <stddef.h>
#include <stdlib.h>

/* Trasher Pool Manager */

struct pool_manager {
  size_t pools_nb;
  struct mem_block **pools;
  char **names;
};

struct mem_block {
  void *data;
  struct mem_block *next;
};

#if GLOBAL_POOL_MANAGER

/**
 * Needed before using trash functions
 */
void init_pool_manager();

#else
/**
 * Get the pool manager structure
 * @return
 */
struct pool_manager *get_pool_manager();

#endif

/**
 * Takes the first pool
 * @param size
 * @return
 */
void *mem(size_t size);

/**
 *
 * @param size
 * @param pool_number
 * @return
 */
void *mem_id(size_t size, size_t pool_id);

/**
 *
 * @param size
 * @param pool_name
 * @return
 */
void *mem_name(size_t size, const char *pool_name);

/**
 * Free the first pool
 */
void free_pool();

void free_id(int pool);

void free_name(const char *pool_name);

/**
 * Free all pools, free the pool_manager
 */
void free_pool_all();

#endif /* !TRASHER_H */
