# Trasher

Trasher is a C memory pool tool.

My projects which used it :

* Unix Shell Like
* Mini Unix Shell Like (smaller and compact)
* Raytracer
* others to add ...

# Usefulness 

Allocate, use, free easily.

> Avoid memory leaks ;)

The trasher is very compact and small, it saved me time and ensure leak free programs in C.

# How to use trasher

* Copy trasher directory into your directory
* Make the library from with the provided Makefile

# Logic - Concept

* Auto-create pools to store requested memory allocated blocks
* Multiple pools
* Allow create blocks in a specific pool:
  * Using an id
  * Using a string (char*) : a tag
* Free all pools
* Free a specific pool (by id or by tag)
* Unspecified tag when pool is created, result to a NULL for the pool tag

# TODO

* Allow user to create Types of allocation
* Rename pools
* Check Pools
* Gets meta-data of pools
* Tests
* Performances
* free correctly names tags
