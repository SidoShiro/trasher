#ifndef TRASHER_H
#define TRASHER_H

/* steddef and stdlib needed to use malloc & cie */
#include <stddef.h>
#include <stdlib.h>

enum mem_type
{
  SOFT = 1,
  HARD = 2,
  ALL = 3
};

/* Trasher Pool Manager */
/*
 * Soft blocks are for temporal memory allocation
 * Hard blocks are for persistant memory (free at exit)
 */
struct pool_manager
{
  struct mem_block *hard;
  struct mem_block *soft;
};

struct mem_block
{
  void *data;
  struct mem_block *next;
};

void init_pool_manager();

struct pool_manager *get_pool();

void *mem_s(size_t size);

void *mem_h(size_t size);

void rm(enum mem_type type);

#endif /* !MEMPOOL_H */
