#pragma once

typedef struct tree{
    void *data;
    struct tree *next;
    struct tree *child;
}tree_t;

tree_t* newTree();
void freeTree(tree_t *tree);