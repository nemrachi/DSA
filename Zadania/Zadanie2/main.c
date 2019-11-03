#include<stdio.h>
#include <stdlib.h>
#include "my_AVL_tree.c"
#include "print_tree.c"
//---------------------------------------------------------------------//
//Poznamka ku komentarom:                                              //
//                                                                     //
//Komentar ku kodu sa nachadza nad danym kodom alebo vedla neho        //                     //
//                                                                     //
//---------------------------------------------------------------------//


int heigh(AVLnode *T, char ch) {
    if (ch == 'l') {
        if (T->left == NULL) {
            return 0;
        } else {
            return max(T->left->lh, T->left->rh) + 1;
        }
    } else if (ch == 'r') {
        if (T->right == NULL) {
            return 0;
        } else {
            return max(T->right->lh, T->right->rh) + 1;
        }
    } else {
        return -1;
    }
}

AVLnode * rotateright(AVLnode *x)
{
    AVLnode *y;
    y=x->left;
    x->left=y->right;
    y->right=x;
    y->parent = x->parent;
    x->parent = y;

    if (x->left != NULL) {
        x->left->parent = x;
    }

    x->lh = heigh(x, 'l');
    x->rh = heigh(x, 'r');
    y->lh=heigh(y, 'l');
    y->rh=heigh(y, 'r');
    return(y);
}

AVLnode * rotateleft(AVLnode *x)
{
    AVLnode *y;
    y=x->right;
    x->right=y->left;

    if (x->right != NULL) {
        x->right->parent = x;
    }

    y->left=x;
    y->parent = x->parent;
    x->parent = y;
    x->lh = heigh(x, 'l');
    x->rh = heigh(x, 'r');
    y->lh=heigh(y, 'l');
    y->rh=heigh(y, 'r');
    return(y);
}

AVLnode * RR(AVLnode *T)
{
    T=rotateleft(T);
    return(T);
}

AVLnode * LL(AVLnode *T)
{
    T=rotateright(T);
    return(T);
}

AVLnode * LR(AVLnode *T)
{
    T->left=rotateleft(T->left);
    T=rotateright(T);

    return(T);
}

AVLnode * RL(AVLnode *T)
{
    T->right=rotateright(T->right);
    T=rotateleft(T);
    return(T);
}

AVLnode * insertNod(AVLnode *T, AVLnode *parent, int x) {
    if(T==NULL) {
        T=(AVLnode*)malloc(sizeof(AVLnode));
        T->data=x;
        T->left=NULL;
        T->right=NULL;
        T->parent = parent;
        T->lh = heigh(T, 'l');
        T->rh = heigh(T, 'r');
    } else if(x >= T->data) {
        T->right= insertNod(T->right, T, x);
        T->rh = heigh(T, 'r');
        if((T->lh-T->rh)==-2) {
            if(x>T->right->data){
                T=RR(T);
            } else{
                T=RL(T);
            }
        }
    } else if(x<T->data) {
        T->left= insertNod(T->left, T, x);
        T->lh = heigh(T, 'l');
        if((T->lh-T->rh)==2) {
            if(x < T->left->data){
                T=LL(T);
            } else{
                T=LR(T);
            }
        }
    }

    return(T);
}

int main() {
    AVLnode *root = NULL;
    short int input = 0;
    int data;

    while (input != 4) {
        printf("Insert(1)\tSearch(2)\tPrint(3)\tEnd(4)\n\nChoose number: ");
        scanf("%hd", &input);

        switch(input) {
            case 1:
                printf("Enter data: ");
                scanf("%d", &data);
                root = insertNod(root, NULL, data);
                break;

            case 2:
                break;

            case 3:
                //print_t(root);
                my_print(root);
                printf("\n");
                break;

            default:
                break;
        }
    }

    return 0;
}