#include "moString.h"

#include <stdio.h>
#include <stdlib.h>

// Allocate memory and initialize the string's contents and metadata
String* str_create() {
        // Allocate space for the string object
        String* str = malloc(sizeof(String));

        // Exit immediately if memory allocation fails
        if (!str) {
                perror("Memory allocation failed");
                exit(1);
        }

        // TODO: initialize metadata, when/if it exists

        // Allocate space for the string's contents
        str->text = buff_create(BUFF_CHAR);

        // Return the initialized String
        return str;
}

// Cleanup
void str_destroy(String* str) {
        // Nothing to do if memory is already free and empty
        if (!str) {
                return;
        }

        // Free and reset the string's contents
        buff_destroy(str->text);
        str->text = NULL;

        // No metadata to reset

        // Free and reset the string object
        free(str);
}

/* Append helper */
static char getNextChar(void* src, int index);

// Add a character to the end of the string. Call with src = NULL and 
// index = -1 to append from stdin
void str_append(String* str, void* src, int pos) {
        // Resize the string's buffer, if necessary
        if (STR_LEN(str) == str->text->capacity) {
                buff_resize(str->text);
        }

        // Add the next character to the string
        STR_AT(str, STR_LEN(str)) = getNextChar(src, pos);

        // Adjust the length of the string accordingly
        STR_LEN(str)++;

        // Adjust the null-terminator
        STR_AT(str, STR_LEN(str)) = '\0';
}

/* getNetChar() helpers */
static char getCharFrom_stream(void* src);
static char getCharFrom_string(char* src, int index);

// Append helper: Call the appropriate getChar function
static char getNextChar(void* src, int index) {
        // If a negative index is given, get the next character from stdin
        if (index < 0) {
                return getCharFrom_stream(src);
        
        // Otherwise, get the character from the indicated index in the string
        } else {
                // Index dereferenced to get its value
                return getCharFrom_string((char*) src, index); 
        }
}

// getNextChar() helper
static char getCharFrom_stream(void* src) {
        // If no input stream specified (src = NULL), get character from stdin. 
        if (!src) {
                return getchar();
        }

        // Otherwise, get input from raw terminal
        // TODO: Implement raw input mode
        return '\0';
}

// getNextChar() helper
static char getCharFrom_string(char* src, int index) {
        // Figure this out in a minute
        return src[index];
}

// Retrieve the string's text. Don't forget to free() after use
char* str_getText(String* str) {
        // Allocate space for the c-string
        char* out = malloc((STR_LEN(str) + 1) * sizeof(char));

        // Copy the contents of the String into a regular character array
        for (int i = 0; i < STR_LEN(str); i++) {
                out[i] = STR_AT(str, i);  // Segfault here
        }

        // Null-terminate
        out[STR_LEN(str)] = '\0';

        // Return the plain char array
        return out;
}