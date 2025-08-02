#ifndef MEM_H
#define MEM_H

#include "handleErrors.h"

#include <stdlib.h>

/* Allocates an array of memory blocks */
void* safeCalloc(size_t quantity, size_t size, Fatality fatalErr);

/* Allocates a single block of memory */
void* safeMalloc(size_t size, Fatality fatalErr);

/* Resizes the memory block */
void* safeRealloc(
        void* oldPtr, size_t oldQty, size_t newQty, 
        size_t elemSize, Fatality fatalErr);

/* Frees the memory block */
void safeFree(void* ptr);

#endif