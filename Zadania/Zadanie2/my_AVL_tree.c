typedef struct AVLnode {
    int data; //hodnota uzla
    struct AVLnode *parent, *left, *right; //smerniky na rodica a lave a prave dieta
    int lh, rh; //vyska lavej vetvy, vyska pravej vetvy
} AVLnode;

int max(int left, int right) {
    if (left > right) {
        return left;
    } else {
        return right;
    }
}

//vypocet vysky pre uzol tree
int height(AVLnode *tree) {
    if (tree == NULL) {
        return 0;
    }
    if (tree->right == NULL) {
        if (tree->left == NULL) {
            return 0;
        } else {
            //ak ma len lave dieta, berie vyssiu hodnotu vysky z neho a pripocita sa 1
            return max(tree->left->lh, tree->left->rh) + 1;
        }
    } else {
        if (tree->left == NULL) {
            //ak ma len prave dieta, berie vyssiu hodnotu vysky z neho a pripocita sa 1
            return max(tree->right->lh, tree->right->rh) + 1;
        } else {
            //ak ma oboje deti, vyberie najvacsiu 1 hodnotu z oboch
            return max(max(tree->left->lh, tree->left->rh), max(tree->right->lh, tree->right->rh)) + 1;
        }
    }
}

AVLnode *right_rotation(AVLnode *tree) {
    AVLnode *left_child = tree->left;
    tree->left = left_child->right;
    left_child->right = tree;

    tree->parent = left_child;
    left_child->parent = tree->parent;

    tree->lh = height(tree);
    tree->rh = height(tree);
    left_child->lh = height(left_child);
    left_child->rh = height(left_child);

    return left_child;
}

AVLnode *left_rotation(AVLnode *tree) {
    AVLnode *right_child = tree->right;
    tree->right = right_child->left;
    right_child->left = tree;

    right_child->parent = tree->parent;
    tree->parent = right_child;

    tree->lh = height(tree);
    tree->rh = height(tree);
    right_child->lh = height(right_child);
    right_child->rh = height(right_child);

    return right_child;
}

AVLnode *left_right_rotation(AVLnode *tree) {
    tree->left = left_rotation(tree->left);
    tree = right_rotation(tree);
    return tree;
}

AVLnode *right_left_rotation(AVLnode *tree) {
    tree->right = right_rotation(tree->left);
    tree = left_rotation(tree);
    return tree;
}

//vytvaranie noveho uzla
AVLnode *new_node(AVLnode *tree, AVLnode *parent, int data) {
    tree = (AVLnode *)malloc(sizeof(AVLnode));
    tree->data = data;
    tree->parent = parent;
    tree->left = NULL;
    tree->right = NULL;
    //vypocet vysky pre lavy a pravy podstrom
    tree->lh = height(tree->left);
    tree->rh = height(tree->right);
    return tree;
}

//vkladanie noveho uzla do stromu
AVLnode *insertNode(AVLnode *tree, AVLnode *parent, int new_data) {
    if (tree == NULL) { //ak dany uzol je NULL, vytvori sa novy uzol
        tree = new_node(tree, parent, new_data);
    } else if (new_data < tree->data) { //ak vlozena hodnota new_data je mensia ako hodnota aktualneho uzla...
        tree->left = insertNode(tree->left, tree, new_data); //...novy uzol bude lave dieta aktualneho uzla
        tree->lh = height(tree); //prepocitanie vysky pre lavy podstrom aktualneho uzla
        //ak faktor vyvazenia (lava vyska - prava vyska) dosiahne hodnotu 2, je strom nevyvazeny a treba robit rotaciu
        if((tree->lh - tree->rh) == 2) {
            if(new_data < tree->left->data){ //ak vlozena hodnota je mensia, ako hodnota laveho dietata aktualneho uzla
                //robi sa prava rotacia aktualneho uzla, aby sa mensie lave dieta dostalo vyssie v strome a vyrovnal
                //sa rozdiel medzi lavym novym uzlom a pravou vetvou stromu
                tree = LL(tree);
            } else{
                //inak sa robi lava rotacia laveho dietata a prava rotacia aktualneho uzla
                tree = LR(tree);
            }
        }

    } else if (new_data > tree->data) { //ak vlozena hodnota new_data je vacsia ako hodnota aktualneho uzla...
        tree->right = insertNode(tree->right, tree, new_data); //...novy uzol bude prave dieta aktualneho uzla
        tree->rh = height(tree); //prepocitanie vysky pre pravy podstrom aktualneho uzla
        //ak faktor vyvazenia (lava vyska - prava vyska) dosiahne hodnotu -2, je strom nevyvazeny a treba robit rotaciu
        if((tree->lh - tree->rh) == -2) {
            if(new_data > tree->right->data){//ak vlozena hodnota je vacsia, ako hodnota praveho dietata aktualneho uzla
                //robi sa lava rotacia aktualneho uzla, aby sa vacsie prave dieta dostalo vyssie v strome a vyrovnal
                //sa rozdiel medzi pravym novym dietatom a lavou vetvou stromu
                tree = RR(tree);
            } else{
                //inak sa robi prava rotacia praveho dietata a lava rotacia aktualneho uzla
                tree = RL(tree);
            }
        }
    }

    printf("lh:%d  rh:%d\n", tree->lh, tree->rh);
    return(tree);
}
