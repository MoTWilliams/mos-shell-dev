#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
        printf("\nWelcome to Mo's Shell!\n\n");
        printf("DEBUG: %d args entered\n", argc);
        
        printf("  ARGS:\n");
        for (int i = 0; argv[i]; i++) {
                printf("  %d. %s\n", i, argv[i]);
        }

        printf("\nGreat job! Goodbye!\n\n");

        return 0;
}