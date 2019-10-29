// uloha3-3.c -- Ema Richnáková, 28.10.2019 09:05

#include <stdio.h>
#include <stdlib.h>

typedef struct NODE {
    int num;
    int lvl;
    struct NODE *left, *right;
} NODE;

NODE * createNewNode(int num, int lvl) {
    NODE * newNode = malloc(sizeof(NODE));
    newNode->num = num;
    newNode->lvl = lvl;
    newNode->left = NULL;
    newNode->right = NULL;
    printf("%d\n", lvl);

    return newNode;
}

//find()

void insert(NODE ** node, int num, int lvl) {
    if (*node == NULL)
        *node = createNewNode(num, lvl);
    else if ((*node)->num > num) {
        if (((*node)->left != NULL) && ((*node)->left->num == num)) {
            printf("%d\n", (*node)->left->lvl);
            return;
        }
        insert(&(*node)->left, num, lvl + 1);
    } else {
        if (((*node)->right != NULL) && ((*node)->right->num == num)) {
            printf("%d\n", (*node)->right->lvl);
        } else if ((*node)->num == num) {
            printf("%d\n", (*node)->lvl);
        } else {
            insert(&(*node)->right, num, lvl + 1);
        }
    }
}

int main() {
    NODE *root = NULL;

    int inputNum;
    char  c;

    while (scanf("%d%c", &inputNum, &c) >= 1) {
        insert(&root, inputNum, 0);
    }

    return 0;
}