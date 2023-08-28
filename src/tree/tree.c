#include <stdlib.h>
#include "tree.h"

tree_t* newTree(){
    tree_t* new_tree = malloc(sizeof(tree_t));
    new_tree->data = NULL;
    new_tree->next = NULL;
    new_tree->child = NULL;
    return new_tree;
}

void freeTree(tree_t *tree){
    if(tree->child != NULL){
        tree_t* current = tree->child;
        while(current != NULL){
            tree_t* tmp = current;
            current = current->next;
            freeTree(tmp);
        }
    }
    free(tree->data);
    free(tree);
}