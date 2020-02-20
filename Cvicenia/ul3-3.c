// uloha3-3.c -- Ema Richnáková, 28.10.2019 09:05

#include <stdio.h>
#include <stdlib.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct AVLnode {
    int data;
    struct AVLnode *parent, *left, *right;
    int lh, rh;
} AVLnode;


//funkcia, ktora vypisuje jednotlive prvky stromu a udaje s nim spojene (data, rodic, deti)
void AVL_print(AVLnode *tree) {
    if(tree != NULL) {
        if (tree->parent == NULL) {
            if ((tree->left == NULL) && (tree->right == NULL)) {
                printf("\ndata:%d\tparent:NONE left:NONE right:NONE\tlh:%d rh:%d", tree->data, tree->lh, tree->rh);
            } else if (tree->left == NULL) {
                printf("\ndata:%d\tparent:NONE left:NONE right:%d\tlh:%d rh:%d", tree->data, tree->right->data, tree->lh, tree->rh);
            } else if (tree->right == NULL) {
                printf("\ndata:%d\tparent:NONE left:%d right:NONE\tlh:%d rh:%d", tree->data, tree->left->data, tree->lh, tree->rh);
            } else {
                printf("\ndata:%d\tparent:NONE left:%d right:%d\tlh:%d rh:%d", tree->data, tree->left->data, tree->right->data, tree->lh, tree->rh);
            }
        } else {
            if ((tree->left == NULL) && (tree->right == NULL)) {
                printf("\ndata:%d\tparent:%d left:NONE right:NONE\tlh:%d rh:%d", tree->data, tree->parent->data, tree->lh, tree->rh);
            } else if (tree->left == NULL) {
                printf("\ndata:%d\tparent:%d left:NONE right:%d\tlh:%d rh:%d", tree->data, tree->parent->data, tree->right->data, tree->lh, tree->rh);
            } else if (tree->right == NULL) {
                printf("\ndata:%d\tparent:%d left:%d right:NONE\tlh:%d rh:%d", tree->data, tree->parent->data, tree->left->data, tree->lh, tree->rh);
            } else {
                printf("\ndata:%d\tparent:%d left:%d right:%d\tlh:%d rh:%d", tree->data, tree->parent->data, tree->left->data, tree->right->data, tree->lh, tree->rh);
            }
        }
        AVL_print(tree->left);
        AVL_print(tree->right);
    }
}

//pomocna funkcia na zistenie maximalnej hodnoty z 2 cisel
int max(int left, int right) {
    if (left > right) {
        return left;
    } else {
        return right;
    }
}

//funkcia vypocitava vysku lavej/pravej vetvy, ktore prisluchaju prvku tree
int AVL_height(AVLnode *tree, char ch) {
    if (ch == 'l') { //zistenie vysky pre lavu vetvu
        if (tree->left == NULL) {
            return 0;
        } else {
            return max(tree->left->lh, tree->left->rh) + 1;
        }
    } else if (ch == 'r') { //zistenie vysky pre pravu vetvu
        if (tree->right == NULL) {
            return 0;
        } else {
            return max(tree->right->lh, tree->right->rh) + 1;
        }
    } else { //ak bola zadana ina hodnota premennej ch
        printf("return -1");
        return -1;
    }
}

//funkcia, ktora vykonava pravu rotaciu prvku tree
AVLnode *AVL_right_rotation(AVLnode *tree) {
    AVLnode *left_child = tree->left;

    //otocenie prvku tree doprava, prvok left_child sa dostane nad prvok tree
    tree->left = left_child->right;
    left_child->right = tree;

    //prepisanie rodicov oboch prvkov
    left_child->parent = tree->parent;
    tree->parent = left_child;
    if (tree->left != NULL) {
        tree->left->parent = tree;
    }

    //prepocitanie vysok pre oba prvky, kedze sa vyrovnavali vetvy (a teda menila sa ich vyska)
    tree->lh = AVL_height(tree, 'l');
    tree->rh = AVL_height(tree, 'r');
    left_child->lh = AVL_height(left_child, 'l');
    left_child->rh = AVL_height(left_child, 'r');

    return left_child;
}

//funkcia, ktora vykonava lavu rotaciu prvku tree
AVLnode *AVL_left_rotation(AVLnode *tree) {
    AVLnode *right_child = tree->right;

    //otocenie prvku tree dolava, prvok right_child sa dostane nad prvok tree
    tree->right = right_child->left;
    right_child->left = tree;

    //prepisanie rodicov oboch prvkov
    right_child->parent = tree->parent;
    tree->parent = right_child;
    if (tree->right != NULL) {
        tree->right->parent = tree;
    }

    //prepocitanie vysok pre oba prvky, kedze sa vyrovnavali vetvy (a teda menila sa ich vyska)
    tree->lh = AVL_height(tree, 'l');
    tree->rh = AVL_height(tree, 'r');
    right_child->lh = AVL_height(right_child, 'l');
    right_child->rh = AVL_height(right_child, 'r');

    return right_child;
}

//funckia na vytvaranie noveho prvku stromu
AVLnode *AVL_new_node(AVLnode *tree, AVLnode *parent, int data) {
    tree = (AVLnode *)malloc(sizeof(AVLnode));
    tree->data = data;
    tree->parent = parent;
    tree->left = NULL;
    tree->right = NULL;
    //vysky pre lave a prave dieta noveho prvku bude vzdy 0 (kedze deti su NULL)
    tree->lh = 0;
    tree->rh = 0;
    return tree;
}

//funkcia na vkladanie prvkov do stromu
AVLnode *AVL_insert(AVLnode *tree, AVLnode *parent, int new_data) {
    if (tree == NULL) { //ak nie je na danom mieste prvok, vyrvori novy prvok a vlozi ho tam
        tree = AVL_new_node(tree, parent, new_data);

    } else if (new_data < tree->data) {
        tree->left = AVL_insert(tree->left, tree, new_data);
        tree->lh = AVL_height(tree, 'l');
        if((tree->lh - tree->rh) == 2) {
            if(new_data <= tree->left->data){
                tree = AVL_right_rotation(tree);

            } else {
                tree->left = AVL_left_rotation(tree->left);
                tree = AVL_right_rotation(tree);
            }
        }

    } else if (new_data > tree->data){ 
        tree->right = AVL_insert(tree->right, tree, new_data); 
        tree->rh = AVL_height(tree, 'r'); 

        if((tree->lh - tree->rh) == -2) {
            if(new_data > tree->right->data){
                tree = AVL_left_rotation(tree);

            } else {
                tree->right = AVL_right_rotation(tree->right);
                tree = AVL_left_rotation(tree);
            }
        }
    }

    return(tree);
}

void find_nearest(AVLnode *tree, int num) {
    if (num < tree->data) {
        if (tree->left != NULL) {
            find_nearest(tree->left, num);
        }

    } else if (num > tree->data) {
        if (tree->right != NULL) {
            find_nearest(tree->right, num);
        }

    } else if (num == tree->data) {
        if (tree->left != NULL) {
            printf("%d\n", tree->left->data);
        } else if (tree->right != NULL) {
            printf("%d\n", tree->right->data);
        } else {
            if (tree->parent == NULL) {
                printf("-1\n");
            } else {
                printf("%d\n", tree->parent->data);
            }
        }
        
    }
}

int main() {
    AVLnode *root = NULL;

    int input;
    char  c;

    while (scanf("%d%c", &input, &c) >= 1) {
        root = AVL_insert(root, NULL, input);
        find_nearest(root, input);
    }
    
    //AVL_print(root);

    return 0;
}