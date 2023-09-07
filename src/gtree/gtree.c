#include "gtree.h"

gtree_t* new_gtree(void *data, unsigned int nb_children){
    gtree_t *tree = malloc(sizeof(gtree_t));
    tree->data = data;
    tree->nb_children = 0;
    tree->max_children = nb_children;
    tree->children = calloc(nb_children, sizeof(gtree_t*));
    return tree;
}

void free_gtree(gtree_t *tree){
    for(unsigned int i = 0; i < tree->nb_children; i++){
        free_gtree(tree->children[i]);
    }
    free(tree->children);
    free(tree);
}

void gtree_resize_nb_children(gtree_t *tree, unsigned int nb_children){
    tree->children = realloc(tree->children, nb_children * sizeof(gtree_t*));
    tree->max_children = nb_children;
}

void gtree_add_child(gtree_t *tree, gtree_t *child){
    if(tree->nb_children < tree->max_children){
        tree->children[tree->nb_children] = child;
        tree->nb_children++;
    }
    else{
        tree->children = realloc(tree->children, tree->max_children * 2 * sizeof(gtree_t*));
        tree->max_children *= 2;
        gtree_add_child(tree, child);
    }
    if(child->depth > tree->depth - 1)
        tree->depth = child->depth + 1;
}

void gtree_remove_child(gtree_t *tree, gtree_t *child){
    unsigned int i = 0;
    unsigned int max_child_depth = 0;
    while(i < tree->nb_children && tree->children[i] != child){
        if(tree->children[i]->depth > max_child_depth)
            max_child_depth = tree->children[i]->depth;
        i++;
    }
    if(i < tree->nb_children){
        for(; i < tree->nb_children - 1; i++){
            tree->children[i] = tree->children[i + 1];
            if(tree->children[i]->depth > max_child_depth)
                max_child_depth = tree->children[i]->depth;
        }

        tree->children[i] = NULL;
        tree->depth = max_child_depth + 1;
        tree->nb_children--;
    }
}

void print_gTree(gtree_t* tree){
    printf("%i(", *((int*)tree->data));
    for(unsigned int i = 0; i < tree->max_children; i++){
        if(tree->children[i] != NULL)
            print_gTree(tree->children[i]);
        if(i < tree->max_children-1)
            printf(", ");
    }
    printf(")");
}