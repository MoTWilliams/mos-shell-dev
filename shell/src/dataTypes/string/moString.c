#include "moString.h"

#include "mem.h"

#include <stdio.h> /* For getchar()*/

/* Allocate memory and initialize the string's contents and metadata */
String* str_create(Fatality isFatal) {
        /* Allocate space for the string object */
        String* str = moMalloc(sizeof(*str), isFatal);

        /* TODO: initialize metadata, when/if it exists */

        /* Allocate space for the string's contents */
        if (str) str->buff = buff_create(BUFF_CHAR, isFatal);

        /* Return the initialized String or NULL */
        return str;
}

/* Cleanup */
void str_destroy(String* str) {
        /* Free and reset the string's contents */
        buff_destroy(str->buff);
        str->buff = NULL;

        /* No metadata to reset */

        /* Free and reset the string object */
        moFree(str);
}

/* Append helper */
static char getNextChar(void* src, int index);

/* Add a character to the end of the string. Call with src = NULL and index = 
 * -1 to append from stdin */
char str_append(String* str, void* src, int pos, Fatality isFatal) {
        char c = '\0';   /* The character to append */
        
        /* Return NULL immediately if str is NULL--not an error */
        if (!str) return c;

        /* Resize the string's buffer, if necessary */
        if (STR_LEN(str) == str->buff->capacity) {
                buff_resize(str->buff, isFatal);
        }

        /* Grab the next character */
        c = getNextChar(src, pos);

        /* Add the next character to the string */
        STR_AT(str, STR_LEN(str)) = c;

        /* Adjust the length of the string accordingly */
        STR_LEN(str)++;

        /* Return the char that was appended */
        return c;
}

/* getNetChar() helpers */
static char getCharFrom_stream(FILE* src);
static char getCharFrom_string(char* src, int index);

/* Append helper: Call the appropriate getChar function */
static char getNextChar(void* src, int index) {
        /* If a negative index is given, get the next character from stdin */
        if (index < 0) {
                return getCharFrom_stream((FILE*) src);
        
        /* Otherwise, get the character from the indicated index in the
         * string */
        } else {
                return getCharFrom_string((char*) src, index); 
        }
}

/* getNextChar() helper */
static char getCharFrom_stream(FILE* src) {
        /* If no input stream specified (src = NULL), get next character from
         * stdin. (Raw terminal later) */ 
        if (!src) {
                return getchar();
        }

        /* Otherwise, get next character from the file */
        return fgetc(src);
}

/* getNextChar() helper */
static char getCharFrom_string(char* src, int index) {
        return src[index];
}

/* Retrieve the string's text. Don't forget to free() after use */
char* str_getText(String* str, Fatality isFatal) {
        /* Allocate space for the c-string. +1 to ensure space for the 
         * null-terminator */
        char* out = moCalloc(STR_LEN(str) + 1, sizeof(*out), isFatal);

        /* Copy the contents of the String into a regular char array */
        if (out) {
                int i; 
                for (i = 0; i < STR_LEN(str); i++) {
                        out[i] = STR_AT(str, i);
                }
        }

        /* Return the plain char array or NULL */
        return out;
}