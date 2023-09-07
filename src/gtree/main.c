#include <stdio.h>
#include "gtree.h"

int main() {
    // Créer un arbre avec une racine contenant un entier
    int root_data = 42;
    gtree_t *root = new_gtree(&root_data, 4);

    // Ajouter des enfants à la racine
    int child1_data = 10;
    gtree_t *child1 = new_gtree(&child1_data, 2);
    gtree_add_child(root, child1);

    int child2_data = 20;
    gtree_t *child2 = new_gtree(&child2_data, 2);
    gtree_add_child(root, child2);

    int child3_data = 30;
    gtree_t *child3 = new_gtree(&child3_data, 0);
    gtree_add_child(root, child3);

    // Supprimer un enfant
    gtree_remove_child(root, child2);
    gtree_add_child(child1, child2);

    // Afficher les données de l'arbre
    printf("Données de la racine : %d\n", *(int *)(root->data));
    printf("Enfants de la racine : %u\n", root->nb_children);

    for (unsigned int i = 0; i < root->nb_children; i++) {
        if (root->children[i] != NULL) {
            printf("Enfant %u : %d\n", i, *(int *)(root->children[i]->data));
        }
    }

    print_gTree(root);

    // Libérer la mémoire
    free_gtree(root);

    return 0;
}
