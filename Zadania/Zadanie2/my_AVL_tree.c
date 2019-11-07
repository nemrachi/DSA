typedef struct AVLnode {
    int data;
    struct AVLnode *parent, *left, *right; //smerniky na rodica a lave a prave dieta
    int lh, rh; //vyska lavej vetvy (lh), vyska pravej vetvy (rh)
} AVLnode;

int AVL_count_lvl = 1;

//funkcia, ktora vypisuje jednotlive uzly stromu
void AVL_search(AVLnode *tree, int wanted_data) {
    if (wanted_data < tree->data) {
        if (tree->left != NULL) {
            AVL_count_lvl++;
            AVL_search(tree->left, wanted_data);
        } else {
            printf("Tree doesn't contain value '%d'\n", wanted_data);
            return;
        }

    } else if (wanted_data > tree->data) {
        if (tree->right != NULL) {
            AVL_count_lvl++;
            AVL_search(tree->right, wanted_data);
        } else {
            printf("Tree doesn't contain value '%d'\n", wanted_data);
            return;
        }

    } else if (wanted_data == tree->data) {
        //printf("Tree contains value '%d'\n", wanted_data);
        printf("contains at lvl: %d\t", AVL_count_lvl);
        AVL_count_lvl = 1;

    } else {
        printf("Tree doesn't contain value '%d'\n", wanted_data);
    }
}

//funkcia, ktora vypisuje jednotlive uzly stromu
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

int max(int left, int right) {
    if (left > right) {
        return left;
    } else {
        return right;
    }
}

//funkcia vypocitava vysku lavej alebo pravej vetvy uzla stromu
int AVL_height(AVLnode *tree, char ch) {
    if (ch == 'l') {
        if (tree->left == NULL) {
            return 0;
        } else {
            return max(tree->left->lh, tree->left->rh) + 1;
        }
    } else if (ch == 'r') {
        if (tree->right == NULL) {
            return 0;
        } else {
            return max(tree->right->lh, tree->right->rh) + 1;
        }
    } else {
        printf("return -1");
        return -1;
    }
}

AVLnode *AVL_right_rotation(AVLnode *tree) {
    AVLnode *left_child = tree->left;
    //otocenie uzla tree doprava, uzol left_child sa dostane nad uzol tree
    tree->left = left_child->right;
    left_child->right = tree;

    //prepisanie rodicov oboch uzlov
    left_child->parent = tree->parent;
    tree->parent = left_child;
    if (tree->left != NULL) {
        tree->left->parent = tree;
    }

    //prepocitanie vysok pre oba uzly, kedze sa vyrovnavali vetvy
    tree->lh = AVL_height(tree, 'l');
    tree->rh = AVL_height(tree, 'r');
    left_child->lh = AVL_height(left_child, 'l');
    left_child->rh = AVL_height(left_child, 'r');

    return left_child;
}

AVLnode *AVL_left_rotation(AVLnode *tree) {
    AVLnode *right_child = tree->right;
    //otocenie uzla tree dolava, uzol right_child sa dostane nad uzol tree
    tree->right = right_child->left;
    right_child->left = tree;

    //prepisanie rodicov oboch uzlov
    right_child->parent = tree->parent;
    tree->parent = right_child;
    if (tree->right != NULL) {
        tree->right->parent = tree;
    }

    //prepocitanie vysok pre oba uzly, kedze sa vyrovnavali vetvy
    tree->lh = AVL_height(tree, 'l');
    tree->rh = AVL_height(tree, 'r');
    right_child->lh = AVL_height(right_child, 'l');
    right_child->rh = AVL_height(right_child, 'r');

    return right_child;
}

//funckia na vytvaranie noveho uzla
AVLnode *AVL_new_node(AVLnode *tree, AVLnode *parent, int data) {
    tree = (AVLnode *)malloc(sizeof(AVLnode));
    tree->data = data;
    tree->parent = parent;
    tree->left = NULL;
    tree->right = NULL;
    //vypocet vysky pre lavy a pravy podstrom
    tree->lh = AVL_height(tree, 'l');
    tree->rh = AVL_height(tree, 'r');
    return tree;
}

//funckia na vkladanie noveho uzla do stromu
AVLnode *AVL_insert(AVLnode *tree, AVLnode *parent, int new_data) {
    if (tree == NULL) { //ak dany uzol je NULL, vytvori sa novy uzol
        tree = AVL_new_node(tree, parent, new_data);

    } else if (new_data < tree->data) { //ak vlozena hodnota new_data je mensia ako hodnota aktualneho uzla...
        tree->left = AVL_insert(tree->left, tree, new_data); //...novy uzol bude v lavej vetve aktualneho uzla
        tree->lh = AVL_height(tree, 'l'); //prepocitanie vysky pre lavy podstrom aktualneho uzla

        //ak faktor vyvazenia (lava vyska - prava vyska) dosiahne hodnotu 2, je strom nevyvazeny a treba robit rotaciu
        if((tree->lh - tree->rh) == 2) {
            if(new_data < tree->left->data){ //ak vlozena hodnota je mensia, ako hodnota laveho dietata aktualneho uzla
                //robi sa prava rotacia aktualneho uzla, aby sa mensie lave dieta dostalo vyssie v strome a vyrovnal
                //sa rozdiel medzi lavym novym uzlom a pravou vetvou stromu
                tree = AVL_right_rotation(tree);
            } else {
                //inak sa robi lava rotacia laveho dietata a prava rotacia aktualneho uzla
                tree->left = AVL_left_rotation(tree->left);
                tree = AVL_right_rotation(tree);
            }
        }

    } else if (new_data >= tree->data) { //ak vlozena hodnota new_data je vacsia alebo rovna ako hodnota aktualneho uzla...
        tree->right = AVL_insert(tree->right, tree, new_data); //...novy uzol bude v pravej vetve aktualneho uzla
        tree->rh = AVL_height(tree, 'r'); //prepocitanie vysky pre pravy podstrom aktualneho uzla

        //ak faktor vyvazenia (lava vyska - prava vyska) dosiahne hodnotu -2, je strom nevyvazeny a treba robit rotaciu
        if((tree->lh - tree->rh) == -2) {
            if(new_data >= tree->right->data){//ak vlozena hodnota je vacsia, ako hodnota praveho dietata aktualneho uzla
                //robi sa lava rotacia aktualneho uzla, aby sa vacsie prave dieta dostalo vyssie v strome a vyrovnal
                //sa rozdiel medzi pravym novym dietatom a lavou vetvou stromu
                tree = AVL_left_rotation(tree);
            } else {
                //inak sa robi prava rotacia praveho dietata a lava rotacia aktualneho uzla
                tree->right = AVL_right_rotation(tree->right);
                tree = AVL_left_rotation(tree);
            }
        }
    }

    return(tree);
}