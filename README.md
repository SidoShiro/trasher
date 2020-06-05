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
* Link the lib to your project

## Tests

```
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

# TODO

* Allow user to create Types of allocation
* Rename pools
* Check Pools
* Gets meta-data of pools
* Performances
* Free correctly names tags

