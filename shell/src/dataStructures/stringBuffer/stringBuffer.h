#ifndef STRINGBUFFER_H
#define STRINGBUFFER_H

typedef struct StringBuffer{
        char* strText;
        int length;
        int capacity;
} StringBuffer;

// Memory allocation and initialization
StringBuffer* strBuf_create();

// Cleanup
void strBuf_destroy(StringBuffer* buff);

// Add a character to the end of the string. Call with src = NULL and 
// index = -1 to append from stdin
void strBuf_append(StringBuffer* buff, void* src, int index);

#endif

