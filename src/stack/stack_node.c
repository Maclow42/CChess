#include "stack_node.h"

stack_node_t* stack_node_new(void* data){
    stack_node_t* new_node = malloc(sizeof(stack_node_t));
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void stack_node_free(stack_node_t* node){
    /*
        * Free the stack node.
        * @node: Pointer to the stack node
    */
    free(node);
}

void stack_node_free_with_data(stack_node_t* node, void (*free_data)(void*)){
    /*
        * Free the stack node and its data.
        * @node: Pointer to the stack node
        * @free_data: Pointer to the function that frees the data
    */
    if(free_data != NULL){
        free_data(node->data);
    }
    free(node);
}
