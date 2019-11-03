typedef struct AVLnode {
    int data; //hodnota uzla
    struct AVLnode *parent, *left, *right; //smerniky na rodica a lave a prave dieta
    int lh, rh; //vyska lavej vetvy, vyska pravej vetvy
} AVLnode;

void my_print(AVLnode * tree) {
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
        my_print(tree->left);
        my_print(tree->right);
    }
}

int max(int left, int right) {
    if (left > right) {
        return left;
    } else {
        return right;
    }
}

//vypocet vysky pre uzol tree
int height(AVLnode *tree, char ch) {
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
        return -1;
    }
}

AVLnode *right_rotation(AVLnode *tree) {
    AVLnode *left_child = tree->left;
    tree->left = left_child->right;
    left_child->right = tree;

    tree->parent = left_child;
    left_child->parent = tree->parent;
    if (tree->left != NULL) {
        tree->left->parent = tree;
    }

    tree->lh = height(tree, 'l');
    tree->rh = height(tree, 'r');
    left_child->lh = height(left_child, 'l');
    left_child->rh = height(left_child, 'r');

    return left_child;
}

AVLnode *left_rotation(AVLnode *tree) {
    AVLnode *right_child = tree->right;
    tree->right = right_child->left;
    right_child->left = tree;

    right_child->parent = tree->parent;
    tree->parent = right_child;
    if (tree->right != NULL) {
        tree->right->parent = tree;
    }

    tree->lh = height(tree, 'l');
    tree->rh = height(tree, 'r');
    right_child->lh = height(right_child, 'l');
    right_child->rh = height(right_child, 'r');

    return right_child;
}

//vytvaranie noveho uzla
AVLnode *new_node(AVLnode *tree, AVLnode *parent, int data) {
    tree = (AVLnode *)malloc(sizeof(AVLnode));
    tree->data = data;
    tree->parent = parent;
    tree->left = NULL;
    tree->right = NULL;
    //vypocet vysky pre lavy a pravy podstrom
    tree->lh = height(tree->left, 'l');
    tree->rh = height(tree->right, 'r');
    return tree;
}

//vkladanie noveho uzla do stromu
AVLnode *insertNode(AVLnode *tree, AVLnode *parent, int new_data) {
    if (tree == NULL) { //ak dany uzol je NULL, vytvori sa novy uzol
        tree = new_node(tree, parent, new_data);
    } else if (new_data < tree->data) { //ak vlozena hodnota new_data je mensia ako hodnota aktualneho uzla...
        tree->left = insertNode(tree->left, tree, new_data); //...novy uzol bude lave dieta aktualneho uzla
        tree->lh = height(tree, 'l'); //prepocitanie vysky pre lavy podstrom aktualneho uzla
        //ak faktor vyvazenia (lava vyska - prava vyska) dosiahne hodnotu 2, je strom nevyvazeny a treba robit rotaciu
        if((tree->lh - tree->rh) == 2) {
            if(new_data < tree->left->data){ //ak vlozena hodnota je mensia, ako hodnota laveho dietata aktualneho uzla
                //robi sa prava rotacia aktualneho uzla, aby sa mensie lave dieta dostalo vyssie v strome a vyrovnal
                //sa rozdiel medzi lavym novym uzlom a pravou vetvou stromu
                tree = right_rotation(tree);
            } else{
                //inak sa robi lava rotacia laveho dietata a prava rotacia aktualneho uzla
                tree->left = left_rotation(tree->left);
                tree = right_rotation(tree);
            }
        }

    } else if (new_data >= tree->data) { //ak vlozena hodnota new_data je vacsia ako hodnota aktualneho uzla...
        tree->right = insertNode(tree->right, tree, new_data); //...novy uzol bude prave dieta aktualneho uzla
        tree->rh = height(tree, 'r'); //prepocitanie vysky pre pravy podstrom aktualneho uzla
        //ak faktor vyvazenia (lava vyska - prava vyska) dosiahne hodnotu -2, je strom nevyvazeny a treba robit rotaciu
        if((tree->lh - tree->rh) == -2) {
            if(new_data > tree->right->data){//ak vlozena hodnota je vacsia, ako hodnota praveho dietata aktualneho uzla
                //robi sa lava rotacia aktualneho uzla, aby sa vacsie prave dieta dostalo vyssie v strome a vyrovnal
                //sa rozdiel medzi pravym novym dietatom a lavou vetvou stromu
                tree = left_rotation(tree);
            } else{
                //inak sa robi prava rotacia praveho dietata a lava rotacia aktualneho uzla
                tree->right = right_rotation(tree->right);
                tree = left_rotation(tree);
            }
        }
    }

    return(tree);
}
