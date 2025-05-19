#include "buffer.h"

#include <stdio.h>
#include <stdlib.h>

#define INIT_SIZE 32

// Allocate memory and initialize the buffer's contents and attributes
Buffer* buff_create(BuffType type) {
        // Allocate space for the buffer and its attributes
        Buffer* buff = malloc(sizeof(Buffer));

        // Exit immediately if memory allocation fails
        // NOTE: Might handle this differently later
        if (!buff) {
                perror("Memory allocation failed");
                exit(1);
        }

        // Initialize metadata
        buff->type = type;
        buff->capacity = INIT_SIZE;
        buff->length = 0;

        // Allocate space for the contents
        buff->data = malloc((buff->capacity + 1) * sizeof(BuffData));

        // Exit immediately if memory allocation fails
        if (!buff->data) {
                perror("Memory allocation failed");
                exit(1);
        }

        // Return the initialized string buffer object
        return buff;
}

// Cleanup
void buff_destroy(Buffer* buff) {
        // Nothing to do if memory is already free and empty
        if (!buff) {
                return;
        }

        // Free and reset the contents
        free(buff->data);
        buff->data = NULL;

        // Reset metadata
        buff->type = BUFF_NOTYPE;
        buff->capacity = INIT_SIZE;
        buff->length = 0;

        // Free and reset the buffer
        free(buff);
}

// Double the capacity of the buffer
void buff_resize(Buffer* buff) {
        // Double capacity metadata value
        buff->capacity *= 2;

        // Re-allocate memory for the larger contents
        buff->data = realloc(
                buff->data, (buff->capacity + 1) * sizeof(BuffData)
        );

        // Exit immediately if memory reallocation fails
        if (!buff->data) {
                perror("Memory re-allocation failed");
                exit(1);
        }
}

