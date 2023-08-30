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

tree_t* addChild(tree_t* tree, void* data){
    tree_t* child = newTree();
    child->data = data;
    child->next = tree->child;
    tree->child = child;
    return child;
}

int getNumberOfChild(tree_t* tree){
    int nb_child = 0;
    tree_t* current = tree->child;
    while(current != NULL){
        nb_child++;
        current = current->next;
    }
    return nb_child;
}