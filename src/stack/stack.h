#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include "stack_node.h"

typedef struct{
    unsigned int size;
    stack_node_t* sentinel;
}stack_t;

stack_t* stack_new();

void stack_free(stack_t* stack);
void stack_free_with_data(stack_t* stack, void (*free_data)(void*));

void stack_push(stack_t* stack, void* data);

void* stack_pop(stack_t* stack);
void* stack_peek(stack_t* stack);

bool stack_is_empty(stack_t* stack);