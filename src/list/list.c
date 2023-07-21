#include <stddef.h>
#include "list.h"

/*############## NODE ##############*/

node_t* newNode(void *data){
    node_t *result = malloc(sizeof(node_t));
    result->data = data;
    result->next = NULL;

    return result;
}

void freeNode(node_t *node){
    free(node);
}

/*############## List ##############*/

list_t* newList(){
    list_t *result = malloc(sizeof(list_t));

    if(result == NULL)
        return NULL;

    result->size = 0;
    result->sentinel = newNode(NULL);
    result->sentinel->next = NULL;

    return result;
}

void freeList(list_t *list){
    node_t *current = list->sentinel;

    while(current!=NULL){
        node_t *tmp = current;
        current = current->next;
        freeNode(tmp);
    }

    free(list);
}

bool pushList(list_t *list, void *data){
    node_t *to_push = newNode(data);

    if(to_push == NULL)
        return false;

    node_t *head = list->sentinel->next;

    list->sentinel->next = to_push;
    to_push->next = head;

    list->size++;

    return true;
}

bool popList(list_t *list, int index){
    if(index < 0 || index >= list->size)
        return false;

    node_t *current = list->sentinel;

    for(int i = 0; i < index; i++)
        current = current->next;

    node_t *to_pop = current->next;
    current->next = to_pop->next;

    freeNode(to_pop);
    list->size--;

    return true;
}

node_t* getIndexList(list_t *list, int index){
    if(index < 0 || index >= list->size)
        return NULL;

    node_t *current = list->sentinel;

    for(int i = 0; i <= index; i++)
        current = current->next;

    return current->data;
}

void printList(list_t *list){
    node_t *current = list->sentinel->next;
    printf("s ->");
    for(int i = 0; i < list->size; i++){
        printf(" %i ->", current->data);
        current = current->next;
    }
    printf("\n");
}