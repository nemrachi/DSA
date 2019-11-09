typedef struct BVSnode {
    int data;
    struct BVSnode *parent, *left, *right; //smerniky na rodica a lave a prave dieta
} BVSnode;

int BVS_count_lvl = 1;

//funkcia, ktora vypisuje jednotlive uzly stromu
void BVS_search(BVSnode *tree, int wanted_data) {
    if (wanted_data < tree->data) {
        if (tree->left != NULL) {
            BVS_count_lvl++;
            BVS_search(tree->left, wanted_data);
        } else {
            printf("Tree doesn't contain value '%d'\n", wanted_data);
            return;
        }

    } else if (wanted_data > tree->data) {
        if (tree->right != NULL) {
            BVS_count_lvl++;
            BVS_search(tree->right, wanted_data);
        } else {
            printf("Tree doesn't contain value '%d'\n", wanted_data);
            return;
        }

    } else if (wanted_data == tree->data) {
        //printf("Tree contains value '%d'\n", wanted_data);
        printf("contains at lvl: %d\t", BVS_count_lvl);
        BVS_count_lvl = 1;

    } else {
        printf("Tree doesn't contain value '%d'\n", wanted_data);
    }
}

//funkcia, ktora vypisuje jednotlive uzly stromu
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

//funckia na vytvaranie noveho uzla
BVSnode *BVS_new_node(BVSnode *parent, int data) {
    BVSnode *tree = (BVSnode *)malloc(sizeof(BVSnode));
    tree->data = data;
    tree->parent = parent;
    tree->left = NULL;
    tree->right = NULL;
    //vypocet vysky pre lavy a pravy podstrom
    return tree;
}

BVSnode *BVS_insert(BVSnode *tree, int new_data) {
    BVSnode **root = &tree;
    BVSnode *parent = NULL;

    while (1) {
        if (*root == NULL) {
            *root = BVS_new_node(parent, new_data);
            break;

        } else if (new_data > (*root)->data) {
            parent = *root;
            root = &(*root)->right;

        } else {
            parent = *root;
            root = &(*root)->left;
        }
    }

    return tree;
}