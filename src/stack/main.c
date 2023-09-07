#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "stack.h"

void printStack(stack_t* stack){
    stack_node_t* current = stack->sentinel->next;
    while(current != NULL){
        printf("%i -> ", *(int*)current->data);
        current = current->next;
    }
    puts("");
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

// Fonction de test pour vérifier si la pile fonctionne correctement.
void test_stack_operations() {
    stack_t* stack = stack_new();

    // Test 1: Vérifiez si la pile est vide initialement.
    assert(stack_is_empty(stack));

    // Test 2: Poussez des éléments sur la pile et vérifiez la taille.
    int data1 = 42;
    stack_push(stack, &data1);
    assert(!stack_is_empty(stack));
    assert(stack->size == 1);

    // Test 3: Vérifiez le sommet de la pile (peek).
    int* peek_data = (int*)stack_peek(stack);
    assert(*peek_data == data1);

    // Test 4: Pop l'élément de la pile.
    int* pop_data = (int*)stack_pop(stack);
    assert(*pop_data == data1);
    assert(stack_is_empty(stack));

    // Test 5: Vérifiez la pile après avoir ajouté plusieurs éléments.
    for (int i = 1; i <= 5; i++) {
        int* j = malloc(sizeof(int));
        *j = i;
        stack_push(stack, j);
        printf("pushed : %i => len : %i\n", i, stack->size);
        print_stack(stack);
        printStack(stack);
        assert(stack->size == i);
    }

    // Test 6: Pop tous les éléments de la pile.
    for (int i = 5; i >= 1; i--) {
        int* popped = (int*)stack_pop(stack);
        printf("popped : %i\n", *popped);
        printStack(stack);
        assert(*popped == i);
    }

    // Test 7: Vérifiez si la pile est vide après avoir tout extrait.
    assert(stack_is_empty(stack));

    stack_free(stack);
}

int main() {
    test_stack_operations();
    printf("Tous les tests ont réussi!\n");
    return 0;
}
