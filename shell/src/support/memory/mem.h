#ifndef MEM_H
#define MEM_H

#include <stdlib.h>

/* Allocates an array of memory blocks */
void* moCalloc(size_t quantity, size_t size, Fatality fatalErr);

/* Allocates a single block of memory */
void* moMalloc(size_t size, Fatality fatalErr);

/* Resizes the memory block */
void* moRealloc(
        void* oldPtr, size_t oldQty, size_t newQty, 
        size_t elemSize, Fatality fatalErr);

/* Frees the memory block */
void moFree(void* ptr);

#endif