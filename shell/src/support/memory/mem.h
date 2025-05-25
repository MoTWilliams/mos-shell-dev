#ifndef MEM_H
#define MEM_H

#include <stdlib.h>

typedef enum {
        ERR_ALLOC,
        ERR_REALLOC
} ErrType;

/* Report memory allocation error */
void error_memory(ErrType err);

/* Allocates an array of memory blocks */
void* moCalloc(size_t quantity, size_t size);

/* Allocates a single block of memory */
void* moMalloc(size_t size);

/* Resizes the memory block */
void* moRealloc(void* oldPtr, size_t oldQty, size_t newQty, size_t elemSize);

/* Frees the memory block */
void moFree(void* ptr);

#endif