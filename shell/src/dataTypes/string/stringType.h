#ifndef MO_STRING_H
#define MO_STRING_H

#include "buffer.h"

#include "handleErrors.h"

#define STR_AT(str, pos) ((str)->buff->data[(pos)].chr)
#define STR_LEN(str) ((str)->buff->length)

/* Always assign STR_TEXT to a char* variable and free() after use, because 
 * str_getText() allocates memory */
#define STR_TEXT(str, isFatal) (str_getText(str, isFatal)) 

typedef struct String {
        /* Contents */
        Buffer* buff;

        /* Metadata */
} String;

/* Memory allocation and initialization */
String* str_create(Fatality isFatal);

/* Cleanup */
void str_destroy(String* str);

/* Add a character to the end of the string. Call with src = NULL and */
/* index = -1 to append from stdin. pos is position in the source string */
char str_append(String* str, void* src, int pos, Fatality isFatal);

/* Retrieve the string's contents - always free() after calling. 
 * Null-terminator is guaranteed due to zeroed memory in safeMalloc() */
char* str_getText(String* str, Fatality isFatal);

#endif