# Trasher

Trasher is a C memory pool tool.

# Usefulness 

Allocate, use, free easily.

> Allocate minimal required

> Avoid memory leaks ;)

> Simple cleaning

The trasher is very compact and small, it saved me time and ensure leak free programs in C.

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


## Tests

```sh
# Should run smoothly
make test_ok

# Should crash : core dumped (double free) 
make test_ko
```

# Logic - Concept

* Auto-create pools to store requested memory allocated blocks
* Multiple pools
* Allow create blocks in a specific pool:
  * Using an id
  * Using a string (char*) : a tag
* Free all pools
* Free a specific pool (by id or by tag)
* Unspecified tag when pool is created, result to a NULL for the pool tag

> This current version of trasher is a mashup of my from-scratch pools used during my studies, many different ways to do
> were coded, I wanted to re-organize the ideas, into one project

## Trasher - Doc

* Lib Functions

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
| `get_pool_manger` | Get the struct which manage the pools and the pools names | - | (struct pool_manager \*) | 


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

## Lacks

* No realloc
* Needs a specific usage of pools in a program

### TODO

* /!\ Allow user to create Types of allocation
* Rename pools
* Performances

