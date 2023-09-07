#pragma once

#include <stdlib.h>
#include <stdio.h>

typedef struct gtree_s{
    void *data;
    unsigned int depth;
    unsigned int nb_children;
    unsigned int max_children;
    struct gtree_s** children;
}gtree_t;

gtree_t* new_gtree(void *data, unsigned int nb_children);
void free_gtree(gtree_t *tree);

void gtree_resize_nb_children(gtree_t *tree, unsigned int nb_children);

void gtree_add_child(gtree_t *tree, gtree_t *child);
void gtree_remove_child(gtree_t *tree, gtree_t *child);

void print_gTree(gtree_t* tree);
