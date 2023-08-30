#pragma once

typedef struct tree{
    void *data;
    struct tree *next;
    struct tree *child;
}tree_t;

tree_t* newTree();
void freeTree(tree_t *tree);
tree_t* addChild(tree_t* tree, void* data);
int getNumberOfChild(tree_t* tree);