#ifndef BUFFER_H
#define BUFFER_H

#include "moErr.h"

/* Forward-declarations */
struct String;

/* Allowed data types for buffer data */

/* Type labels */
typedef enum {
        BUFF_NOTYPE,
        BUFF_CHAR,
        BUFF_STRING
} BuffType;

/* Data object */
typedef union {
        char chr;
        struct String* strPtr;
} BuffData;

/* Buffer definition */

typedef struct Buffer {
        /* Contents */
        BuffData* data;

        /* Metadata */
        BuffType type;
        int capacity;
        int length;
} Buffer;

/* Memory allocation and initialization */
Buffer* buff_create(BuffType type, Fatality isFatal);

/* Cleanup */
void buff_destroy(Buffer* buff);

/* Double the capacity */
void buff_resize(Buffer* buff, Fatality isFatal);

#endif