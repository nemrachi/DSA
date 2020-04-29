#include <stdio.h>
#include <stdlib.h>

typedef struct BVSnode {
    int data; 
    struct BVSnode *parent, *left, *right;
} BVSnode;

int BVS_count_lvl = 1;

//vyhladavacia funkcia
void BVS_search(BVSnode *tree, int wanted_data) {
    if (wanted_data < tree->data) { //ak su hladane data mensie ako aktualny uzol...
        if (tree->left != NULL) { //...a je na lavo prvok
            BVS_count_lvl++;
            BVS_search(tree->left, wanted_data); //posuva sa dolava
        } else {
            printf("Tree doesn't contain value '%d'\n", wanted_data);
            return;
        }

    } else if (wanted_data > tree->data) { //ak su hladane data vacsie ako aktualny uzol...
        if (tree->right != NULL) { //...a je na pravo prvok
            BVS_count_lvl++;
            BVS_search(tree->right, wanted_data); //posuva sa doprava
        } else {
            printf("Tree doesn't contain value '%d'\n", wanted_data);
            return;
        }

    } else if (wanted_data == tree->data) { //ak najde hladany prvok, vypise aj jeho uroven v strome
        printf("contains at lvl: %d\t", BVS_count_lvl);
        BVS_count_lvl = 1;
        return;

    } else {
        printf("Tree doesn't contain value '%d'\n", wanted_data);
        return;
    }
}

//funkcia, ktora vypisuje jednotlive prvky stromu a udaje s nim spojene (data, rodic, deti)
void BVS_print(BVSnode *tree) {
    if(tree != NULL) {
        if (tree->parent == NULL) {
            if ((tree->left == NULL) && (tree->right == NULL)) {
                printf("\ndata:%d\tparent:NONE left:NONE right:NONE", tree->data);
            } else if (tree->left == NULL) {
                printf("\ndata:%d\tparent:NONE left:NONE right:%d", tree->data, tree->right->data);
            } else if (tree->right == NULL) {
                printf("\ndata:%d\tparent:NONE left:%d right:NONE", tree->data, tree->left->data);
            } else {
                printf("\ndata:%d\tparent:NONE left:%d right:%d", tree->data, tree->left->data, tree->right->data);
            }
        } else {
            if ((tree->left == NULL) && (tree->right == NULL)) {
                printf("\ndata:%d\tparent:%d left:NONE right:NONE", tree->data, tree->parent->data);
            } else if (tree->left == NULL) {
                printf("\ndata:%d\tparent:%d left:NONE right:%d", tree->data, tree->parent->data, tree->right->data);
            } else if (tree->right == NULL) {
                printf("\ndata:%d\tparent:%d left:%d right:NONE", tree->data, tree->parent->data, tree->left->data);
            } else {
                printf("\ndata:%d\tparent:%d left:%d right:%d", tree->data, tree->parent->data, tree->left->data, tree->right->data);
            }
        }
        BVS_print(tree->left);
        BVS_print(tree->right);
    }
}

//funckia na vytvaranie noveho prvku stromu
BVSnode *BVS_new_node(BVSnode *parent, int data) {
    BVSnode *tree = malloc(sizeof(BVSnode));
    tree->data = data;
    tree->parent = parent;
    tree->left = NULL;
    tree->right = NULL;
    return tree;
}

//funkcia na vkladanie prvkov do stromu
BVSnode *BVS_insert(BVSnode *tree, int new_data) {
    BVSnode **root = &tree; //aktualny prvok
    BVSnode *parent = NULL; //rodic aktualneho prvku

    while (1) {
        if (*root == NULL) { //ak nie je na danom mieste prvok, vyrvori novy prvok a vlozi ho tam
            *root = BVS_new_node(parent, new_data);
            return tree;

        } else if (new_data <= (*root)->data) { //ak vkladane data su mensie alebo rovne ako aktualny prvok, posuva sa dolava
            parent = *root;
            root = &(*root)->left;

        } else { //ak vkladane data su vacsie ako aktualny prvok, posuva sa doprava
            parent = *root;
            root = &(*root)->right;
        }
    }
}