typedef struct BVSnode {
    int data;
    struct BVSnode *parent, *left, *right; //smerniky na rodica a lave a prave dieta
    int lh, rh; //vyska lavej vetvy (lh), vyska pravej vetvy (rh)
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
        BVS_print(tree->left);
        BVS_print(tree->right);
    }
}

int BVS_max(int left, int right) {
    if (left > right) {
        return left;
    } else {
        return right;
    }
}

//funkcia vypocitava vysku lavej alebo pravej vetvy uzla stromu
int BVS_height(BVSnode *tree, char ch) {
    if (ch == 'l') {
        if (tree->left == NULL) {
            return 0;
        } else {
            return BVS_max(tree->left->lh, tree->left->rh) + 1;
        }
    } else if (ch == 'r') {
        if (tree->right == NULL) {
            return 0;
        } else {
            return BVS_max(tree->right->lh, tree->right->rh) + 1;
        }
    } else {
        return -1;
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
//    tree->lh = BVS_height(tree, 'l');
//    tree->rh = BVS_height(tree, 'r');
    return tree;
}

//funckia na vkladanie noveho uzla do stromu
//BVSnode *BVS_insert(BVSnode *tree, BVSnode *parent, int new_data) {
//    if (tree == NULL) { //ak dany uzol je NULL, vytvori sa novy uzol
//        tree = BVS_new_node(tree, parent, new_data);
//
//    } else if (new_data < tree->data) { //ak vlozena hodnota new_data je mensia ako hodnota aktualneho uzla...
//        tree->left = BVS_insert(tree->left, tree, new_data); //...novy uzol bude v lavej vetve aktualneho uzla
//        tree->lh = BVS_height(tree, 'l'); //prepocitanie vysky pre lavy podstrom aktualneho uzla
//
//    } else { //ak vlozena hodnota new_data je vacsia alebo rovna ako hodnota aktualneho uzla...
//        tree->right = BVS_insert(tree->right, tree, new_data); //...novy uzol bude v pravej vetve aktualneho uzla
//        tree->rh = BVS_height(tree, 'r'); //prepocitanie vysky pre pravy podstrom aktualneho uzla
//    }
//
//    return(tree);
//}

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