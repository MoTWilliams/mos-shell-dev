#include <stdio.h>
#include <stdlib.h>

#define INIT_SIZE 8

typedef struct Buffer {
        char* str;
        int length;
        int capacity;
} Buffer;

Buffer* buffer_create();
char getNextChar();
void buffer_populate(Buffer* buff);
void buffer_destroy(Buffer* buff);

int main() {
        Buffer* input = buffer_create();

        printf("Enter something\n> ");
        fflush(stdout);

        buffer_populate(input);

        printf("You entered: %s\n", input->str);

        buffer_destroy(input);
        return 0;
}

Buffer* buffer_create() {
        Buffer* buff = malloc(sizeof(Buffer));

        buff->length = 0;
        buff->capacity = INIT_SIZE;

        // Allocate the actual buffer
        buff->str = malloc(buff->capacity);

        return buff;
}

void buffer_resizeIfNeeded(Buffer* buff) {
        if (buff->length == buff->capacity) {
                buff->capacity *= 2;
                buff->str = realloc(buff->str, buff->capacity);
        }
}

char getNextChar() {
        return getchar();
}

void buffer_populate(Buffer* buff) {
        char c;
        int i;
        for (i = 0; (c = getNextChar()) != '\n'; i++) {
                buffer_resizeIfNeeded(buff);
                buff->str[i] = c;
                buff->length++;
        }
        buffer_resizeIfNeeded(buff);
        buff->str[i] = '\0';
}

void buffer_destroy(Buffer* buff) {
        // Free the string and reset values
        free(buff->str);
        buff->str = NULL;
        buff->length = 0;
        buff->capacity = INIT_SIZE;
        
        // Free the buffer itself
        free(buff);
        buff = NULL;
}