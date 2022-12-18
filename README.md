# Trasher

Trasher is a C memory pool tool.

![Trasher Logo](doc/trasher_logo.png)

# Usefulness 

Allocate, use, free easily.

> Allocate minimal required

> Avoid memory leaks ;)

> Simple cleaning

The trasher is very compact and small, it saved me time and ensure leak free programs in C.

Allocate to specific pools, free them as easily.

# How to use trasher

* Copy trasher directory into your directory
* Make the library from with the provided Makefile
* Ensure the access to **trasher.h** for your code
* Link the lib to your project

```sh:
# If the library is in the standard lib folder of your system:

gcc  test.c -ltrasher

# OR (specify path):

gcc  test.c -L/path/to/lib/lib -ltrasher

# OR (local path):

gcc test.c -L. -ltrasher
```

### Basic in code (usage)

1. Allocate simple pool
   `void* = mem(size_t bytes)`
2. Allocate id pool 
   `void* = mem_id(size_t bytes)`
3. Allocate name pool
   `void* = mem_name(size_t bytes, char *pool_name)`
4. Free a pool, default pool, any id pool or any name pool
   `free_pool()`, `free_id(size_t id)`, `free_name(char *pool_name)`
5. Final Free at end of the program
   `free_pool_all()`

### Advanced

* Rename pool `pool_rename(char *original_name, char *new_name)`

## Tests

```sh
# all tests
make test

# Should run smoothly
make test_ok

# Should crash : core dumped (double free) 
make test_ko
```

## Logic - Concept

Trasher is a memory pool tool for C programs. It allows you to:

* Allocate memory in a simple and efficient way, using the minimal required resources.
* Avoid memory leaks by releasing used memory effectively.
* Easily clean up allocated memory using Trasher's release functions.

Trasher is very compact and lightweight, it saves time and ensures leak-free C programs.

It is possible to allocate memory in specific pools and release them easily.

Trasher works by automatically creating pools to store allocated memory blocks. It allows for the creation of multiple pools, as well as blocks in a specific pool using an ID or a name (string). It is also possible to release all pools, a specific pool (by ID or name), or all allocated memory using the final release function. If no name is specified when creating a pool, it will be assigned to NULL.

> This current version of Trasher is a compilation of the memory pool tools I developed from scratch during my studies. It includes many different approaches and techniques that I wanted to organize and bring together in one project.

## Trasher - Doc

* User Functions

| Function | Desc. | Args. | Return |
|----------|-------|-------|--------|
| `mem`      | Automatic allocation to the first pool | size (size_t) : allocation size in bytes | (void \*) : allocated block |
| `mem_name` | Recommended function to use to allocate blocks, create or re-use the pool asked with pool_name | size (size_t) : allocation size in bytes, pool_name (const char \*pool_name) : pool name | (void \*) : allocated block |
| `mem_id`   | Allocate to a specific pool channel | size (size_t) allocation size in bytes, pool_id (size_t) : the id of the pool | (void \*) : allocated block |
| `free_pool` | Free the first pool | - | - |
| `free_id` | Free the pool id, if has name removes it | pool (size_t) : the pool id | - |
| `free_name` | Free the pool with pool_name as name | pool_name (const char \*) : the pool name | - |
| `free_pool_all` | Remove all pools, reset pool_manager | - | - |
| `pool_status` | Get printed view of the pools, used mainly for debug | - | (many prints) |
| `pool_give_number_blocks` | Give number of allocated blocks in a pool | - | - |
| `pool_give_name_from_id` | Retrieve name of a pool from id of pool, NULL if error/not found | id (size_t) | name (char \*) |
| `pool_give_id_from_name` | Retrieve id of a pool using the name of the pool, return -1 if not found or error | name (char \*) | id (ssize_t) |
| `pool_rename` | Rename a pool name to a new name | char \* source_name, char \* new_name | int |


* Dev Level Lib Functions

| Function | Desc. | Args. | Return |
|----------|-------|-------|--------|
| `get_pool_manger` | Get the struct which manage the pools and the pools names | - | (struct pool_manager \*) | 
| `pool_status_debug` | Get printed view of the pools, used mainly for debug, on stderr | - | (many prints) |


* Lib data structs

* pool_manager struct

| Field | Type | Desc. |
|-------|------|-------|
| `pools_nb` | size_t | Number of current pools |
| `pools` | struct mem_block \*\* | Array of pools first blocks |
| `names` | char \*\* | Array of each name for specific pool |

* mem_block struct

| Field | Type | Desc. |
|-------|------|-------|
| `data`  | void \* | Allocation address given by malloc |
| `next`  | struct mem_block \* | Next memory block (linked list) |
| `data_size` | size_t | Save the size of the allocated block | 

## Notes

Behaviour of mixing id pools with named pools : the "merge effect"

If you have a "PoolA" named pool. Add block of 128.
Then add a block of 512 to pool id 1:
The pool status will be:
```
[0] : [NULL] NULL
[1] : ["PoolA"] 128 -> 512
```

> Pool 0 has no name, it's reserved for the mem() and free_pool() functions | and mem_id(XXX, 0)

### Performances

* **v1.1** (use of tail blocks)
  ```
  Average time for     malloc lib : 0.000564
  Average time for    trasher lib : 0.000259
  ```

* **v1.0**
  ```
  Average time for     malloc lib : 0.000564
  Average time for    trasher lib : 0.010310
  ```

### TODO

* Realloc (wrapper ?)
