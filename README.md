# Trasher

Trasher is a C memory pool tool.

My projects which used it :

* Unix Shell Like
* Mini Unix Shell Like (smaller and compact)
* Raytracer
* others to add ...

# Usefullness 

Allocate, use, free easily.

> Avoid memory leaks

# Logic

These files are a merged of my different implementations of trasher.

I defined two different types of allocation SOFT and HARD :

* SOFT : memory which is not supposed to stay in the heap for a long time
* HARD : memory which has to stay allocated until the end

The trasher will wrap the malloc function.

This design is useful for Shells, SOFT is used to build AST, do all execution and shell allocation for a command.
At the end of the execution of the command all of the memory allocated for this specified command is free.

HARD allocation is useful for memory which will have to stay available for the program during a long time, sometimes until the end of the execution. In a case of shell, it will be environment variables and other stuff.

# TODO

* Allow user to create Types of allocation

