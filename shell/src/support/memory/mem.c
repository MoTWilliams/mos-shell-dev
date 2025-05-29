#include "mem.h"
#include "textStylesAndColors.h"

#include <stdio.h>      /* for calloc() */
#include <string.h>     /* for memcpy() */
#include <unistd.h>     /* for getpid() */

/* Memory allocation error */
void error_memory(ErrType err) {
        char* errMsg = "";

        switch (err) {
                case ERR_ALLOC:
                        errMsg = "Error allocating memory";
                        break;
                case ERR_REALLOC:
                        errMsg = "Error reallocating memory";
                        break;
                default:
                        errMsg = "Unknown memory error";
                        break;
        }

        printf(FG_RED "[" BOLD "MEMORY ERROR" UNBOLD "]" RESET);
        fflush(stdout);
        fprintf(stderr, "(pid: %d) %s\n", getpid(), errMsg);
}

void* moCalloc(size_t quantity, size_t size) {
        void* newPtr = calloc(quantity, size);

        /* Don't limp along if memory allocation fails. Just exit immediately */
        if (!newPtr) {
                error_memory(ERR_ALLOC);
                exit(1);
        }

        return newPtr;
}

/* moMalloc() exists for semantic clarity, but it currently zeros out the 
 * allocated space like moCalloc(1, size). This may change in the future. */
void* moMalloc(size_t size) {
        return moCalloc(1, size);
}

/* When calling this, remember that resizing from larger to smaller truncates 
 * the data */
void* moRealloc(void* oldPtr, size_t oldQty, size_t newQty, size_t elemSize) {
        /* Allocate the new block with zeroed-out memory */
        void* newPtr = calloc(newQty, elemSize);

        /* Exit immediately if memory allocation fails */
        if (!newPtr) {
                error_memory(ERR_REALLOC);
                exit(1);
        }

        /* If the old pointer is empty (all NULL), no need to copy anything */
        if (!oldPtr) {
                return newPtr;
        }

        /* Copy up to the smaller of old/new sizes to avoid overrunning either 
         * buffer. */
        if (oldQty > newQty) {
                memcpy(newPtr, oldPtr, newQty * elemSize);
        } else {
                memcpy(newPtr, oldPtr, oldQty * elemSize);
        }

        /* Free and empty the old pointer */
        moFree(oldPtr);

        /* return the resized memory block */
        return newPtr;
}

/* This is just free with a safety check */
void moFree(void* ptr) {
        /* Nothing to free if memory is already free or NULL */
        if (!ptr) {
                return;
        }

        free(ptr);
}