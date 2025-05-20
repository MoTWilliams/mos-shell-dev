#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct Node {
        int val;
        struct Node* next;
} Node;

typedef struct List {
        Node* head;
} List;

// These (adapted to a tree) will be useful for parsing
List* list_create();
void list_destroy(List* list);
void list_append(List* list, int newVal);
void list_prepend(List* list, int newVal);

// These will get more useful when dealing with shell history and whatnot
Node* list_searchForNode(List* list, int key);
int list_searchForIndex(List* list, int key);

void list_print(List* list);

int main() {
        List* list = list_create();

        list_prepend(list, 4);
        list_prepend(list, 3);
        list_prepend(list, 2);
        list_prepend(list, 1);
        list_prepend(list, 0);

        list_print(list);

        int key = 5;
        int index = list_searchForIndex(list, key);
        if (index < 0) {
                printf("%d not found\n", key);
        } else {
                printf("%d found at index %d\n", key, index);
        }

        list_destroy(list);
        list = NULL;
        return 0;
}

List* list_create() {
        // Allocate memory for the list
        List* list = malloc(sizeof(List));
        
        // Exit immediately if allocation fails
        if (!list) {
                perror("Memory allocation failed");
                exit(1);
        }

        // Initialize the empty list
        list->head = NULL;

        // Return the new list
        return list;
}

void list_destroy(List* list) {
        // Nothing to do if the list is already free and empty
        if (!list) {
                return;
        }

        // Store a reference to the current node
        Node* current = list->head;
        
        // Destroy all nodes of the list first
        while (current) {
                // Store a reference to the next node
                Node* next = current->next;

                // Destroy the current node
                free(current);
                
                // Move on to the next node
                current = next;
        }

        // Then destroy the list
        free(list);
}


void list_append(List* list, int newVal) {
        // Allocate space for the new node
        Node* newNode = malloc(sizeof(Node));

        // Exit immediately if allocation fails
        if (!newNode) {
                perror("Memory allocation failed");
                exit(1);
        }

        // Initialize the new node's value
        newNode->val = newVal;

        // Store a reference to the current node (Initialize to head)
        Node* current = list->head;

        // Special case: The list is empty--new node is now head
        if (!list->head) {
                list->head = newNode;
        // Otherwise, find the end of the list and add the new node
        } else {
                // Iterate to the end of the list
                while (current->next) {
                        current = current->next;
                }

                // After the loop, current is now the last node in the list. We 
                // then point the last node in the list's next pointer at the 
                // new node
                current->next = newNode;
        }
}

void list_prepend(List* list, int newVal) {
        // Allocate space for the new node
        Node* newNode = malloc(sizeof(Node));

        // Exit immediately if allocation fails
        if (!newNode) {
                perror("Memory allocation failed");
                exit(1);
        }

        // Initialize the new node's value
        newNode->val = newVal;

        // Point the new node at the head of the list
        newNode->next = list->head;

        // New node is now head
        list->head = newNode;
}


// Use to retrieve the actual node
Node* list_searchForNode(List* list, int key) {
        // Store a reference to the current node (Initialize to list head)
        Node* current = list->head;

        // Iterate through the list
        while (current) {
                // Return immediately when the node is found
                if (current->val == key) {
                        return current;
                }

                // Otherwise, move on to the next node
                current = current->next;
        }

        // This only executes if the node is not found
        return NULL;
}

// Use to get the index of the node
int list_searchForIndex(List* list, int key) {
        // Store a reference to the current node (Initialize to list head)
        Node* current = list->head;

        // Iterate through the list
        for (int i = 0; current; i++) {
                // Return immediately when the node is found
                if (current->val == key) {
                        return i;
                }

                // Otherwise, move on to the next node
                current = current->next;
        }

        // This only executes if the node is not found
        return -1;
}


void list_print(List* list) {
        Node* current = list->head;

        while (current) {
                printf("%d", current->val);

                Node* next = current->next;
                if (next) {
                        printf(" -> ");
                } else {
                        printf("\n");
                }

                current = next;
        }
}
