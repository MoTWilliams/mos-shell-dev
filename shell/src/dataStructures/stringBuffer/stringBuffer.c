#include "stringBuffer.h"

#include <stdio.h>
#include <stdlib.h>

#define INIT_SIZE 32

// Allocate memory and initialize the buffer's contents and attributes
StringBuffer* strBuf_create() {
        // Allocate space for the buffer and its attributes
        StringBuffer* buff = malloc(sizeof(StringBuffer));

        // Exit immediately if memory allocation fails
        if (!buff) {
                perror("Memory allocation failed");
                exit(1);
        }

        // Initialize attributes
        buff->length = 0;
        buff->capacity = INIT_SIZE;

        // Allocate space for the string itself
        buff->strText = malloc(buff->capacity * sizeof(char));

        // Exit immediately if memory allocation fails
        if (!buff->strText) {
                perror("Memory allocation failed");
                exit(1);
        }

        // Return the initialized string buffer object
        return buff;
}

// Cleanup
void strBuf_destroy(StringBuffer* buff) {
        // Free and reset the string
        free(buff->strText);
        buff->strText = NULL;
        
        // Reset attributes
        buff->length = 0;
        buff->capacity = INIT_SIZE;

        // Free and reset the buffer object
        free(buff);
}

// Append helper
char getCharFrom_stream(void* src) {
        // If no input stream specified (src = NULL), get character from stdin. 
        if (!src) {
                return getchar();
        }

        // Otherwise, get input from raw terminal
        // TODO: Implement raw input mode
}

// Append helper
char getCharFrom_string(char* src, int index) {
        // Figure this out in a minute
        return src[index];
}

// Append helper: Call the appropriate getChar function
char getNextChar(void* src, int index) {
        // If a negative index is given, get the next character from stdin
        if (index < 0) {
                return getCharFrom_stream(src);
        
        // Otherwise, get the character from the indicated index in the string
        } else {
                // Index dereferenced to get its value
                return getCharFrom_string((char*) src, index); 
        }
}

// Append helper: Double the capacity of the buffer
void strBuf_resize(StringBuffer* buff) {
        // Double the capacity
        buff->capacity *= 2;

        // Re-allocate memory for the larger string
        buff->strText = realloc(buff->strText, buff->capacity * sizeof(char));

        // Exit immediately if memory reallocation fails
        if (!buff->strText) {
                perror("Memory re-allocation failed");
                exit(1);
        }
}

// Add a character to the end of the string. Call with src = NULL and 
// index = -1 to append from stdin
void strBuf_append(StringBuffer* buff, void* src, int index) {
        // Resize the buffer, if necessary
        if (buff->length == buff->capacity) {
                strBuf_resize(buff);
        }

        // Add the next character to the string
        buff->strText[buff->length] = getNextChar(src, index);

        // Adjust the length of the string accordingly
        buff->length++;

        // Adjust null-terminator, resizing the buffer if needed
        if (buff->length == buff->capacity) {
                strBuf_resize(buff);
        }
        buff->strText[buff->length] = '\0';
}
