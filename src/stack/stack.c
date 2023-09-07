#include <stdio.h>
#include "stack.h"

stack_t* stack_new(){
    stack_t* new_stack = malloc(sizeof(stack_t));
    new_stack->size = 0;
    new_stack->sentinel = stack_node_new(NULL);
    return new_stack;
}

void stack_free(stack_t* stack){
    /*
        * Free the stack.
        * @stack: Pointer to the stack
    */
    stack_node_t* current = stack->sentinel->next;
    while(current != NULL){
        stack_node_t* tmp = current;
        current = current->next;
        stack_node_free(tmp);
    }
    stack_node_free(stack->sentinel);
    free(stack);
}

void print_stack(stack_t* stack) {
    if (stack == NULL || stack_is_empty(stack)) {
        printf("Empty Stack\n");
        return;
    }

    printf("Stack: ");
    stack_node_t* current = stack->sentinel->next;

    while (current != NULL) {
        printf("%p -> ", current->data);
        current = current->next;
    }

    printf("NULL\n");
}

void stack_free_with_data(stack_t* stack, void (*free_data)(void*)){
    /*
        * Free the stack.
        * @stack: Pointer to the stack
        * @free_data: Pointer to the function that frees the data
    */
    stack_node_t* current = stack->sentinel->next;
    while(current != NULL){
        stack_node_t* tmp = current;
        current = current->next;
        stack_node_free_with_data(tmp, free_data);
    }
    stack_node_free(stack->sentinel);
    free(stack);
}

void stack_push(stack_t* stack, void* data){
    /*
        * Push the given data to the stack
        * @stack: Pointer to the stack
        * @data: the data to push
    */
    stack_node_t* new_node = stack_node_new(data);
    new_node->next = stack->sentinel->next;
    stack->sentinel->next = new_node;
    stack->size++;
}

void* stack_pop(stack_t* stack){
    /*
        * Pop the given data from the stack and return it
        * @stack: Pointer to the stack
    */
    if(stack->size == 0){
        return NULL;
    }
    stack_node_t* tmp = stack->sentinel->next;
    stack->sentinel->next = tmp->next;
    void* data = tmp->data;
    stack_node_free(tmp);
    stack->size--;
    return data;
}
void* stack_peek(stack_t* stack){
    /*
        * Peek the given data from the stack and return it
        * @stack: Pointer to the stack
    */
    if(stack->size == 0){
        return NULL;
    }
    return stack->sentinel->next->data;
}

bool stack_is_empty(stack_t* stack){
    return stack->size == 0;
}