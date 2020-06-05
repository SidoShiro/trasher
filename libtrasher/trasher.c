/* Trasher Pool */
#include "trasher.h"

struct pool_manager *get_pool_manager() {
  static struct pool_manager *manager = NULL;
  if (!manager) {
    manager = malloc(sizeof(struct pool_manager));
    manager->pools_nb = 0;
    manager->pools = NULL;
    manager->names = NULL;
  }
  return manager;
}

static void* add_to(struct mem_block *head, struct mem_block *newBlock) {
  if (head == NULL) {
    head = newBlock;
    return head;
  }
  struct mem_block *iter_blk = head;
  while (iter_blk->next != NULL)
    iter_blk = iter_blk->next;
  iter_blk->next = newBlock;
  return head;
}

void *mem(size_t size) {
  // Go on first pool (default mem)
  struct pool_manager *pm = get_pool_manager();
  if (pm) {
    if (pm->pools_nb == 0 && pm->pools == NULL) {
      pm->pools = malloc(sizeof(struct mem_block *));
      pm->pools_nb = 1;
      pm->pools[0] = malloc(sizeof(struct mem_block));
      pm->pools[0]->data = malloc(size);
      pm->pools[0]->next = NULL;
      pm->pools[0]->data_size = size;
      pm->names = malloc(sizeof(char *));
      pm->names[0] = NULL;
      return pm->pools[0]->data;
    } else {
      struct mem_block *blk = malloc(sizeof(struct mem_block));
      blk->data = malloc(size);
      blk->next = NULL;
      blk->data_size = size;
      pm->pools[0] = add_to(pm->pools[0], blk);
      return blk->data;
    }
  }
  return NULL;
}

void *mem_id(size_t size, size_t pool_id) {
  struct pool_manager *pm = get_pool_manager();
  if (pm) {
    if (pm->pools_nb == 0 && pm->pools == NULL) {
      pm->pools = malloc(sizeof(struct mem_block *));
      pm->names = malloc(sizeof(char *));
      pm->names[0] = NULL;
    }
    if (pm->pools_nb <= pool_id) {
      pm->pools = realloc(pm->pools, sizeof(struct mem_block *) * (pool_id + 1));
      pm->names = realloc(pm->names, sizeof(char *) * (pool_id + 1));

      for (size_t i = pm->pools_nb; i < pool_id; i++) {
        pm->pools[i] = NULL;
        pm->names[i] = NULL;
      }
      pm->pools_nb = pool_id + 1;
      pm->pools[pool_id] = malloc(sizeof(struct mem_block));
      pm->pools[pool_id]->data = malloc(size);
      pm->pools[pool_id]->next = NULL;
      pm->pools[pool_id]->data_size = size;
      return pm->pools[pool_id]->data;
    } else {
      struct mem_block *blk = malloc(sizeof(struct mem_block));
      blk->data = malloc(size);
      blk->next = NULL;
      blk->data_size = size;
      pm->pools[pool_id] = add_to(pm->pools[pool_id], blk);
      return blk->data;
    }
  }
  return NULL;
}

void *mem_name(size_t size, const char *pool_name) {
  if (!pool_name)
    return NULL;
  struct pool_manager *pm = get_pool_manager();
  if (pm) {
    if (pm->pools_nb == 0 && pm->pools == NULL) {
      // If first call, init pools and names
      pm->pools = malloc(sizeof(struct mem_block *));
      pm->names = malloc(sizeof(char *));
      pm->names[0] = NULL;
    }

    // Search in current pools if name match
    size_t pool_id = 0;
    for (; pool_id < pm->pools_nb; pool_id++)
      if (pm->names[pool_id] != NULL && 0 == strcmp(pool_name, pm->names[pool_id])) {
        struct mem_block *blk = malloc(sizeof(struct mem_block));
        blk->data = malloc(size);
        blk->data_size = size;
        blk->next = NULL;
        pm->pools[pool_id] = add_to(pm->pools[pool_id], blk);
        return blk->data;
      }

    // Let mem() use first slot, if wasn't created yet, so increment to 1
    pool_id == 0 ? pool_id++: 0;

    // No name match, create a new pool
    if (pm->pools_nb <= pool_id) {
      pm->pools = realloc(pm->pools, sizeof(struct mem_block *) * (pool_id + 1));
      pm->names = realloc(pm->names, sizeof(char *) * (pool_id + 1));

      for (size_t i = pm->pools_nb; i < pool_id; i++) {
        pm->pools[i] = NULL;
        pm->names[i] = NULL;
      }

      // Set Name
      size_t len = strlen(pool_name);
      char *new_pool_name = malloc(sizeof(char) * (len + 1));
      strcpy(new_pool_name, pool_name);
      new_pool_name[len] = '\0';

      pm->pools_nb = pool_id + 1;
      pm->pools[pool_id] = malloc(sizeof(struct mem_block));
      pm->pools[pool_id]->data = malloc(size);
      pm->pools[pool_id]->data_size = size;
      pm->pools[pool_id]->next = NULL;
      pm->names[pool_id] = new_pool_name;
      return pm->pools[pool_id]->data;
    }
  }
  return NULL;
}


static void rm_list_block(struct mem_block *head) {
  if (head == NULL)
    return;
  struct mem_block *next = head->next;
  while (next) {
    free(head->data);
    free(head);
    head = next;
    next = next->next;
  }
  if (head != NULL) {
    free(head->data);
    free(head);
  }
}

void free_pool() {
  struct pool_manager *pm = get_pool_manager();
  if (pm->pools_nb >= 1 && pm->pools[0] != NULL) {
    printf("* FREE");
    rm_list_block(pm->pools[0]);
    pm->pools[0] = NULL;
  }

}

void free_id(int pool) {
  struct pool_manager *pm = get_pool_manager();
  if (pm->pools_nb >= pool && pm->pools[pool] != NULL) {
    rm_list_block(pm->pools[pool]);
    pm->pools[pool] = NULL;
  }
}

void free_name(const char *pool_name) {
  if (!pool_name)
    return;
  struct pool_manager *pm = get_pool_manager();
  size_t pool_id = 0;
  for (; pool_id < pm->pools_nb; pool_id++) {
    if (pm->names[pool_id] != NULL && 0 == strcmp(pool_name, pm->names[pool_id])) {
      printf("DEBUG - Remove pool %d", pool_id);
      rm_list_block(pm->pools[pool_id]);
      pm->pools[pool_id] = NULL;
      return;
    }
  }
}

// TODO function delete_name_from_manager()
// To delete the pool with the name/id in the manager

void free_pool_all() {
  struct pool_manager *pm = get_pool_manager();
  size_t pool_id = 0;
  for (; pool_id < pm->pools_nb; pool_id++) {
    if (pm->pools[pool_id] != NULL) {
      printf(" * FREE %zu\n", pool_id);
      rm_list_block(pm->pools[pool_id]);
      pm->pools[pool_id] = NULL;
    }
  }
  // TODO Free manager data
  for (size_t i = 0; i < pm->pools_nb; i++) {
    free(pm->names[i]);
  }
  free(pm->names);
  pm->names = NULL;
  free(pm->pools);
  pm->pools = NULL;
  pm->pools_nb = 0;
  free(pm);
  pm = NULL;
}

void pool_status() {
  struct pool_manager *pm = get_pool_manager();
  if (pm == NULL) {
    printf("Pool Manager is NULL - cannot give status!");
    return;
  }
  printf("\n--- Pools Manager ---\n Pools : %4zu\n", pm->pools_nb);
  if (!pm->pools)
    return;
  for (size_t i = 0; i < pm->pools_nb; i++) {
    if (pm->pools[i] == NULL) {
      printf("[%2zu] : [%s] : NULL\n", i, pm->names && pm->names[i] != NULL ? pm->names[i] : "NULL");
    } else {
      printf("[%2zu] : [%s] : ", i, pm->names && pm->names[i] != NULL ? pm->names[i] : "NULL");
      struct mem_block *h = pm->pools[i];
      while (h) {
        printf("%zu", h->data_size);
        h = h->next;
        if (h)
          printf(" > ");
      }
      printf("\n");
    }
  }
  printf("---               ---\n");
}

/* OLD FIXME SOFT HARD version
void rm(enum mem_type type) {
  switch (type) {
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
 */
