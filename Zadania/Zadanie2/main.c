#include<stdio.h>
#include <stdlib.h>
#include "print_tree.c"


int height(AVLnode *T)
{
    int lh,rh;
    if(T==NULL)
        return(0);

    if(T->left==NULL)
        lh=0;
    else
        lh=1+T->left->bf;

    if(T->right==NULL)
        rh=0;
    else
        rh=1+T->right->bf;

    if(lh>rh)
        return(lh);

    return(rh);
}

AVLnode * rotateright(AVLnode *x)
{
    AVLnode *y;
    y=x->left;
    x->left=y->right;
    y->right=x;
    x->bf=height(x);
    y->bf=height(y);
    return(y);
}

AVLnode * rotateleft(AVLnode *x)
{
    AVLnode *y;
    y=x->right;
    x->right=y->left;
    y->left=x;
    x->bf=height(x);
    y->bf=height(y);

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

int BF(AVLnode *T)
{
    int lh,rh;
    if(T==NULL)
        return(0);

    if(T->left==NULL)
        lh=0;
    else
        lh=1+T->left->bf;

    if(T->right==NULL)
        rh=0;
    else
        rh=1+T->right->bf;

    return(lh-rh);
}

void preorder(AVLnode *T)
{
    if(T!=NULL)
    {
        printf("%d(Bf=%d)",T->val,BF(T));
        preorder(T->left);
        preorder(T->right);
    }
}

void inorder(AVLnode *T)
{
    if(T!=NULL)
    {
        inorder(T->left);
        printf("%d(Bf=%d)",T->val,BF(T));
        inorder(T->right);
    }
}

AVLnode * insertNode(AVLnode *T, int x)
{
    if(T==NULL)
    {
        T=(AVLnode*)malloc(sizeof(AVLnode));
        T->val=x;
        T->left=NULL;
        T->right=NULL;
    }
    else
    if(x > T->val)		// insert in right subtree
    {
        T->right= insertNode(T->right, x);
        if(BF(T)==-2) {
            if(x>T->right->val)
                T=RR(T);
            else
                T=RL(T);
        }
    }
    else
    if(x<T->val)
    {
        T->left= insertNode(T->left, x);
        if(BF(T)==2) {
            if(x < T->left->val)
                T=LL(T);
            else
                T=LR(T);
        }
    }

    T->bf=height(T);

    return(T);
}

int main() {
    AVLnode *root = NULL;
    int x,root_data,i;
    short int input = 0;

    while (input != 4) {
        printf("Create(1)\nInsert(2)\nPrint(3)\nEnd(4)\n\nEnter number: ");
        scanf("%hd", &input);

        switch(input) {
            case 1:
                printf("Enter root: ");
                scanf("%d",&root_data);
                printf("\nEnter tree data:");
                root=NULL;
                for(i=0; i < root_data; i++)
                {
                    scanf("%d",&x);
                    root= insertNode(root, x);
                }
                break;

            case 2:
                printf("\nEnter data:");
                scanf("%d",&x);
                root= insertNode(root, x);
                break;

            case 3:
                printf("\nPreorder sequence:\n");
                preorder(root);
//                printf("\n\nInorder sequence:\n");
//                inorder(root);
//                printf("\n");
                print_t(root);
                break;

            default:
                break;
        }
    }

    return 0;
}