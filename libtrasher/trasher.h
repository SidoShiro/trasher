#ifndef TRASHER_H
#define TRASHER_H

/* steddef and stdlib needed to use malloc & cie */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

/* Trasher Pool Manager */

struct pool_manager {
  size_t pools_nb;
  struct mem_block **pools;
  char **names;
  struct mem_block **tails; // Get to last block of a pool to enhance speed of block creation
};

struct mem_block {
  void *data;
  struct mem_block *next;
  size_t data_size;
};

/**
 * Get the pool manager structure
 * @return
 */
struct pool_manager *get_pool_manager(char reset);


/**
 * Takes the first pool
 * @param size
 * @return
 */
void *mem(size_t size);

/**
 * Get pointer to memory of size size, from pool number pool_id
 * If the pool_id doesn't exist, create a new pools in the pools array to match
 * the require pool_number
 * @param size
 * @param pool_number
 * @return
 */
void *mem_id(size_t size, size_t pool_id);

/**
 * Get Data, of size size, from pool tagged by pool_name
 * If the pool_name doesn't exist, create a new pool tagged with pool_name
 * @param size size_t size of the required block
 * @param pool_name const char* the name of the pool name
 * @return
 */
void *mem_name(size_t size, const char *pool_name);

/**
 * Free the first pool
 */
void free_pool();

/**
 * Free the pool number, remove pool tag (in names if not NULL)
 * @param pool size_t pool id
 */
void free_id(size_t pool);

/**
 * Free pool tagged by pool_name, remove pool_name tag
 * @param pool_name const char* name of the pool
 */
void free_name(const char *pool_name);

/**
 * Free all pools, free the pool_manager
 */
void free_pool_all();

/**
 * Give current pool manager status
 * Tracked pools
 * Number of allocations
 * Size of pools allocations
 */
void pool_status();

/**
 * Same as pool_status but on stderr
 */
void pool_status_debug();

/**
 * Give id of a pool using a name
 * Return a pointer to the string in the meta-array of names
 */
char *pool_give_name_from_id(size_t id);

/**
 * Give number of blocks in a pool
 * Return the number of allocated blocks
 */
ssize_t pool_give_number_blocks(size_t id);

/**
 * Give pool id from name, if name doesn't exist in pools or error return -1
 * @param name
 * @return
 */
ssize_t pool_give_id_from_name(char *name);

/**
 * Rename an existing pool (source name) with the new name
 * @param source_name
 * @param new_name
 * @return 1 if done, 0 if source_name doesn't exist, -1 if error
 */
int pool_rename(char *source_name, char *new_name);

#endif /* !TRASHER_H */
