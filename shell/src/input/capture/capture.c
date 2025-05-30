#include "input.h"

#include "moString.h"
#include "mem.h"
#include "moErr.h"
#include "textStylesAndColors.h"

#include <stdio.h>

static char* capture_interactive(void);
static char* capture_batch(char* srcPath);

char* input_capture(InputMode mode, char* srcPath) {
        /* Call the appropriate capture function */
        switch (mode) {
                case MODE_INTERACTIVE:
                        return capture_interactive();
                case MODE_BATCH:
                        return capture_batch(srcPath);
                default:
                        printf("Input error\n");
                        return NULL;
        }
}

static char* capture_interactive(void) {
        String* inputBuff = str_create(FATAL);   /* Store the raw input */
        char* inputCStr = NULL;             /* Store the c-string to return */
        char c = '\0';                      /* Store the last char captured */
        
        /* Null-termination is handled by str_append(). Just append until we 
         * hit a newline or EOF */
        while ((c = str_append(inputBuff, NULL, -1, FATAL)) != EOF) {
                /* Stop appending and trim newline, if present */
                if (c == '\n') {
                        STR_AT(inputBuff, STR_LEN(inputBuff) - 1) = '\0';
                        STR_LEN(inputBuff)--;
                        break;
                }
        }

        /* Grab the c-string from the string buffer */
        inputCStr = str_getText(inputBuff, FATAL);

        /* Destroy the buffer */
        str_destroy(inputBuff);

        /* Just return the plain c-string */
        return inputCStr;
}

static char* capture_batch(char* srcPath) {
        FILE* file = NULL;                /* Pointer for batch file stream */
        String* buff = str_create(FATAL); /* Buffer to hold raw input */
        char* input = NULL;               /* c-string to return */
        char curr[2] = {'\0','\0'};       /* The current char from stream */
        char next[2] = {'\0','\0'};       /* The next char from stream */

        /* If NULL, file couldn't be opened. */
        if (!(file = fopen(srcPath, "r"))) {
                fprintf(stderr,"Couldn't read from file\n");
                return NULL;
        }

        /* Prime with the first character from the file. return immediately on 
         * error or empty file */
        if((curr[0] = fgetc(file)) == EOF) {
                str_destroy(buff);
                return NULL;
        }

        /* Append chars one-at-a-time from the file stream */
        while (1) {
                /* Advance the lookahead. Leave the loop when the end of the 
                 * file is reached */
                if((next[0] = fgetc(file)) == EOF) {
                        break;
                }

                /* If an escaped newline is detected, toss both the backslash 
                 * and the newline*/
                if (curr[0] == '\\' && next[0] == '\n') {
                        curr[0] = fgetc(file);
                        next[0] = fgetc(file);
                        continue;
                }

                /* Append the current char */
                str_append(buff, curr, 0, FATAL);

                /* Move on to the next char */
                curr[0] = next[0];
        }

        /* Close the file when finished reading */
        if (fclose(file) == EOF) {
                fprintf(stderr,"Failed to close file");
                return NULL;
        }

        /* Get the c-string from the String buffer */
        input = str_getText(buff, FATAL);

        /* Destroy the buffer */
        str_destroy(buff);

        /* Return the c-string */
        return input;
}