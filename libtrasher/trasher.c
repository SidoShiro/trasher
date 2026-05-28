/* Trasher Pool */
#include "trasher.h"

struct pool_manager *get_pool_manager(char reset) {
  static struct pool_manager *manager = NULL;
  if (reset) {
    manager = NULL;
    return NULL;
  }
  if (!manager) {
    manager = malloc(sizeof(struct pool_manager));
    if (manager) {
      manager->pools_nb = 0;
      manager->pools = NULL;
      manager->names = NULL;
      manager->tails = NULL;
    }
  }
  return manager;
}

static int ensure_capacity(struct pool_manager *pm, size_t pool_id) {
  if (pm->pools_nb <= pool_id) {
    size_t new_nb = pool_id + 1;
    struct mem_block **new_pools = realloc(pm->pools, sizeof(struct mem_block *) * new_nb);
    char **new_names = realloc(pm->names, sizeof(char *) * new_nb);
    struct mem_block **new_tails = realloc(pm->tails, sizeof(struct mem_block *) * new_nb);

    if (!new_pools || !new_names || !new_tails) {
      // If one fails, we might have a problem if we don't roll back, 
      // but let's at least keep what we have.
      if (new_pools) pm->pools = new_pools;
      if (new_names) pm->names = new_names;
      if (new_tails) pm->tails = new_tails;
      return 0;
    }

    pm->pools = new_pools;
    pm->names = new_names;
    pm->tails = new_tails;

    for (size_t i = pm->pools_nb; i < new_nb; i++) {
      pm->pools[i] = NULL;
      pm->names[i] = NULL;
      pm->tails[i] = NULL;
    }
    pm->pools_nb = new_nb;
  }
  return 1;
}

static struct mem_block *create_block(size_t size) {
  struct mem_block *blk = malloc(sizeof(struct mem_block));
  if (!blk) return NULL;
  blk->data = malloc(size);
  if (!blk->data) {
    free(blk);
    return NULL;
  }
  blk->next = NULL;
  blk->data_size = size;
  return blk;
}

void *mem(size_t size) {
  struct pool_manager *pm = get_pool_manager(0);
  if (!pm) return NULL;

  if (!ensure_capacity(pm, 0)) return NULL;

  struct mem_block *blk = create_block(size);
  if (!blk) return NULL;

  if (pm->pools[0] == NULL) {
    pm->pools[0] = blk;
    pm->tails[0] = blk;
  } else {
    pm->tails[0]->next = blk;
    pm->tails[0] = blk;
  }
  return blk->data;
}

void *mem_id(size_t size, size_t pool_id) {
  struct pool_manager *pm = get_pool_manager(0);
  if (!pm) return NULL;

  if (!ensure_capacity(pm, pool_id)) return NULL;

  struct mem_block *blk = create_block(size);
  if (!blk) return NULL;

  if (pm->pools[pool_id] == NULL) {
    pm->pools[pool_id] = blk;
    pm->tails[pool_id] = blk;
  } else {
    pm->tails[pool_id]->next = blk;
    pm->tails[pool_id] = blk;
  }
  return blk->data;
}

void *mem_name(size_t size, const char *pool_name) {
  if (!pool_name) return NULL;
  struct pool_manager *pm = get_pool_manager(0);
  if (!pm) return NULL;

  if (!ensure_capacity(pm, 0)) return NULL;

  // Search for existing pool name
  for (size_t i = 0; i < pm->pools_nb; i++) {
    if (pm->names[i] != NULL && strcmp(pool_name, pm->names[i]) == 0) {
      return mem_id(size, i);
    }
  }

  // Find empty slot (excluding slot 0 reserved for mem())
  size_t pool_id = 0;
  for (size_t i = 1; i < pm->pools_nb; i++) {
    if (pm->names[i] == NULL && pm->pools[i] == NULL) {
      pool_id = i;
      break;
    }
  }

  if (pool_id == 0) {
    pool_id = pm->pools_nb;
  }

  if (!ensure_capacity(pm, pool_id)) return NULL;

  pm->names[pool_id] = strdup(pool_name);
  if (!pm->names[pool_id]) return NULL;

  return mem_id(size, pool_id);
}

static void rm_list_block(struct mem_block *head) {
  while (head) {
    struct mem_block *next = head->next;
    free(head->data);
    free(head);
    head = next;
  }
}

void free_pool() {
  free_id(0);
}

void free_id(size_t pool) {
  struct pool_manager *pm = get_pool_manager(0);
  if (pm && pm->pools && pool < pm->pools_nb) {
    if (pm->pools[pool]) {
      rm_list_block(pm->pools[pool]);
      pm->pools[pool] = NULL;
      pm->tails[pool] = NULL;
    }
    if (pm->names && pm->names[pool]) {
      free(pm->names[pool]);
      pm->names[pool] = NULL;
    }
  }
}

void free_name(const char *pool_name) {
  if (!pool_name) return;
  struct pool_manager *pm = get_pool_manager(0);
  if (!pm || !pm->names) return;
  for (size_t i = 0; i < pm->pools_nb; i++) {
    if (pm->names[i] != NULL && strcmp(pool_name, pm->names[i]) == 0) {
      free_id(i);
      return;
    }
  }
}

void free_pool_all() {
  struct pool_manager *pm = get_pool_manager(0);
  if (!pm) return;

  for (size_t i = 0; i < pm->pools_nb; i++) {
    if (pm->pools && pm->pools[i]) {
      rm_list_block(pm->pools[i]);
    }
    if (pm->names && pm->names[i]) {
      free(pm->names[i]);
    }
  }
  free(pm->names);
  free(pm->pools);
  free(pm->tails);
  free(pm);
  get_pool_manager(1);
}

void pool_status() {
  struct pool_manager *pm = get_pool_manager(0);
  if (!pm) {
    printf("Pool Manager is NULL - cannot give status!\n");
    return;
  }
  printf("\n--- Pools Manager ---\n Pools : %4zu\n", pm->pools_nb);
  if (!pm->pools) return;
  for (size_t i = 0; i < pm->pools_nb; i++) {
    printf("[%2zu] : [%s] : ", i, (pm->names && pm->names[i]) ? pm->names[i] : "NULL");
    struct mem_block *h = pm->pools[i];
    if (!h) {
      printf("NULL\n");
    } else {
      while (h) {
        printf("%zu", h->data_size);
        h = h->next;
        if (h) printf(" > ");
      }
      printf("\n");
    }
  }
  printf("---               ---\n");
}

void pool_status_debug() {
  struct pool_manager *pm = get_pool_manager(0);
  if (!pm) {
    dprintf(2, "Pool Manager is NULL - cannot give status!\n");
    return;
  }
  dprintf(2, "\n--- Pools Manager ---\n Pools : %4zu\n", pm->pools_nb);
  if (!pm->pools) return;
  for (size_t i = 0; i < pm->pools_nb; i++) {
    dprintf(2, "[%2zu] : [%s] : ", i, (pm->names && pm->names[i]) ? pm->names[i] : "NULL");
    struct mem_block *h = pm->pools[i];
    if (!h) {
      dprintf(2, "NULL\n");
    } else {
      while (h) {
        dprintf(2, "%zu", h->data_size);
        h = h->next;
        if (h) dprintf(2, " > ");
      }
      dprintf(2, "\n");
    }
  }
  dprintf(2, "---               ---\n");
}

char *pool_give_name_from_id(size_t id) {
  struct pool_manager *pm = get_pool_manager(0);
  if (pm && id < pm->pools_nb && pm->names) {
    return pm->names[id];
  }
  return NULL;
}

ssize_t pool_give_id_from_name(const char *name) {
  if (!name) return -1;
  struct pool_manager *pm = get_pool_manager(0);
  if (!pm || !pm->names) return -1;
  for (size_t i = 0; i < pm->pools_nb; i++) {
    if (pm->names[i] && strcmp(pm->names[i], name) == 0) {
      return i;
    }
  }
  return -1;
}

ssize_t pool_give_number_blocks(size_t id) {
  struct pool_manager *pm = get_pool_manager(0);
  if (!pm || !pm->pools) return -1;
  if (id >= pm->pools_nb) return -2;
  
  ssize_t n = 0;
  struct mem_block *curr = pm->pools[id];
  while (curr) {
    n++;
    curr = curr->next;
  }
  return n;
}

int pool_rename(const char *source_name, const char *new_name) {
  if (!source_name || !new_name) return -1;
  struct pool_manager *pm = get_pool_manager(0);
  if (!pm || !pm->names) return 0;

  for (size_t i = 0; i < pm->pools_nb; i++) {
    if (pm->names[i] && strcmp(pm->names[i], source_name) == 0) {
      char *tmp = strdup(new_name);
      if (!tmp) return -1;
      free(pm->names[i]);
      pm->names[i] = tmp;
      return 1;
    }
  }
  return 0;
}
