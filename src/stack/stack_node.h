#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef struct stack_node_s{
    void* data;
    struct stack_node_s * next;
}stack_node_t;

stack_node_t* stack_node_new(void* data);
void stack_node_free(stack_node_t* node);
void stack_node_free_with_data(stack_node_t* node, void (*free_data)(void*));
