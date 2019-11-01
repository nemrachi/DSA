#include<stdio.h>
#include <stdlib.h>
#include "my_AVL_tree.c"
//---------------------------------------------------------------------//
//Poznamka ku komentarom:                                              //
//                                                                     //
//Komentar ku kodu sa nachadza nad danym kodom alebo vedla neho        //                     //
//                                                                     //
//---------------------------------------------------------------------//

int height(AVLnode *T) {
    if (T == NULL) {
        return 0;
    }
    if (T->right == NULL) {
        if (T->left == NULL) {
            return 0;
        } else {
            return max(T->left->lh, T->left->rh) + 1;
        }
    } else {
        if (T->left == NULL) {
            return max(T->right->lh, T->right->rh) + 1;
        } else {
            return max(max(T->left->lh, T->left->rh), max(T->right->lh, T->right->rh)) + 1;
        }
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
    x->lh = height(x);
    x->rh = height(x);
    y->lh=height(y);
    y->rh=height(y);
    return(y);
}

AVLnode * rotateleft(AVLnode *x)
{
    AVLnode *y;
    y=x->right;
    x->right=y->left;
    y->left=x;
    y->parent = x->parent;
    x->parent = y;
    x->lh = height(x);
    x->rh = height(x);
    y->lh=height(y);
    y->rh=height(y);
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

void preorder(AVLnode *T)
{
    if(T!=NULL)
    {
        printf("%d(Bf=%d)",T->data,(T->lh-T->rh));
        preorder(T->left);
        preorder(T->right);
    }
}

void inorder(AVLnode *T)
{
    if(T!=NULL)
    {
        inorder(T->left);
        printf("%d(Bf=%d)",T->data,(T->lh-T->rh));
        inorder(T->right);
    }
}

AVLnode * insertNod(AVLnode *T, AVLnode *parent, int x) {
    if(T==NULL) {
        T=(AVLnode*)malloc(sizeof(AVLnode));
        T->data=x;
        T->left=NULL;
        T->right=NULL;
        T->parent = parent;
        T->lh = height(T->left);
        T->rh = height(T->right);
    } else if(x > T->data) {
        //T->parent->rh = T->parent->rh + 1;
        T->right= insertNod(T->right, T, x);
        T->rh = height(T);
        if((T->lh-T->rh)==-2) {
            if(x>T->right->data){
                T=RR(T);
            } else{
                T=RL(T);
            }
        }
    } else if(x<T->data) {
        //T->lh = T->lh + 1;
        T->left= insertNod(T->left, T, x);
        T->lh = height(T);
        if((T->lh-T->rh)==2) {
            if(x < T->left->data){
                T=LL(T);
            } else{
                T=LR(T);
            }
        }
    }

    printf("lh:%d  rh:%d\n", T->lh, T->rh);

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
                print_t(root);
                break;

            default:
                break;
        }
    }

    return 0;
}