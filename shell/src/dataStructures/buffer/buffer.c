#include "buffer.h"

#include "mem.h"

#define INIT_SIZE 32

/* Allocate memory and initialize the buffer's contents and attributes */
Buffer* buff_create(BuffType type, Fatality isFatal) {
        /* Allocate space for the buffer and its attributes */
        Buffer* buff = safeMalloc(sizeof(*buff), isFatal);

        /* Immediately return NULL if allocation fails*/
        if (!buff) return NULL;

        /* Initialize metadata */
        buff->type = type;
        buff->capacity = INIT_SIZE;
        buff->length = 0;

        /* Allocate space for the contents */
        buff->data = safeCalloc(buff->capacity + 1, sizeof(*buff->data), isFatal);

        /* Return the initialized buffer object */
        return buff;
}

/* Cleanup */
void buff_destroy(Buffer* buff) {
        /* Do nothing if buff is already free and NULL*/
        if (!buff) return;

        /* Free and reset the contents */
        safeFree(buff->data);
        buff->data = NULL;

        /* Reset metadata */
        buff->type = BUFF_NOTYPE;
        buff->capacity = INIT_SIZE;
        buff->length = 0;

        /* Free and reset the buffer */
        safeFree(buff);
}

/* Double the capacity of the buffer */
Buffer* buff_resize(Buffer* buff, Fatality isFatal) {
        /* New capacity will be double the current. Add 1 to ensure space for 
         * the null-terminator */
        size_t oldCapacity = buff->capacity;
        size_t newCapacity = buff->capacity * 2;
        
        /* Do nothing and immediately return NULL if buff is NULL */
        if (!buff) return NULL;

        /* Re-allocate memory for the larger contents. + 1 for null-teminator */
        buff->data = safeRealloc(
                buff->data, oldCapacity + 1, newCapacity + 1, 
                sizeof(*buff->data), isFatal
        );

        /* Resize the capacity metadata */
        buff->capacity = newCapacity;

        /* return the resized buffer or NULL */
        return buff;
}

