#include "buffer.h"

#include "mem.h"

#define INIT_SIZE 32

/* Allocate memory and initialize the buffer's contents and attributes */
Buffer* buff_create(BuffType type, Fatality isFatal) {
        /* Allocate space for the buffer and its attributes */
        Buffer* buff = moMalloc(sizeof(*buff), isFatal);

        /* If allocation successful, initialize the buffer */
        if (buff) {
                /* Initialize metadata */
                buff->type = type;
                buff->capacity = INIT_SIZE;
                buff->length = 0;

                /* Allocate space for the contents */
                buff->data = moCalloc(buff->capacity + 1, 
                                                sizeof(*buff->data), isFatal);
        }

        /* Return the initialized buffer object or NULL */
        return buff;
}

/* Cleanup */
void buff_destroy(Buffer* buff) {
        /* Free and reset the contents */
        moFree(buff->data);
        buff->data = NULL;

        /* Reset metadata */
        buff->type = BUFF_NOTYPE;
        buff->capacity = INIT_SIZE;
        buff->length = 0;

        /* Free and reset the buffer */
        moFree(buff);
}

/* Double the capacity of the buffer */
Buffer* buff_resize(Buffer* buff, Fatality isFatal) {
        /* New capacity will be double the current. Add 1 to ensure space for 
         * the null-terminator */
        size_t oldCapacity = buff->capacity;
        size_t newCapacity = buff->capacity * 2;

        /* Reallocation error is handled elsewhere. This just doesn't fire if a 
         * NULL pointer is passed in*/
        if (buff) {
                /* Re-allocate memory for the larger contents. + 1 for 
                 * null-teminator */
                buff->data = moRealloc(
                        buff->data, oldCapacity+1, newCapacity+1, 
                        sizeof(*buff->data), isFatal
                );

                /* Resize the capacity metadata */
                buff->capacity = newCapacity;
        }

        /* return the resized buffer or NULL */
        return buff;
}

