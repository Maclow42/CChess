#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef struct node_s{
    void *data;
    struct node_s *next;
}node_t;

typedef struct{
    unsigned int size;
    node_t *sentinel;
}list_t;

node_t* newNode(void *data);
void freeNode(node_t *node);

/*############## List ##############*/

list_t* newList();
void freeList(list_t *list);
bool pushList(list_t *list, void *data);
bool popList(list_t *list, int index);
node_t* getIndexList(list_t *list, int index);
void printList(list_t *list);