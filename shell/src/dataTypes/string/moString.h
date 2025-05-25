#ifndef MO_STRING_H
#define MO_STRING_H

#include "buffer.h"

#define STR_AT(str, pos) ((str)->text->data[(pos)].chr)
#define STR_LEN(str) ((str)->text->length)
#define STR_TEXT(str) (str_getText(str)) /* Always assign to a char* variable */
                                         /* and free() after use, because */ 
                                         /* str_getText() allocates memory */

typedef struct String {
        /* Contents */
        char* actualText; /* TODO: This will certainly get a new name, but I 
         * think it may be easier to deal with strings if I have an easy way to 
         * just...have the text from the buffer in a regular c-string */

        /* Helper structure */
        Buffer* text;

        /* Metadata */
} String;

/* Memory allocation and initialization */
String* str_create(void);

/* Cleanup */
void str_destroy(String* str);

/* Add a character to the end of the string. Call with src = NULL and */
/* index = -1 to append from stdin. pos is position in the source string */
void str_append(String* str, void* src, int pos);

/* Retrieve the string's contents - always free() after calling. 
 * Null-terminator is guaranteed due to zeroed memory in moMalloc() */
char* str_getText(String* str);

#endif