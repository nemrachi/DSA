#include<stdio.h>
#include <stdlib.h>
#include "my_AVL_tree.c"
#include "red-black_tree.c"
//---------------------------------------------------------------------//
//Poznamka ku komentarom:                                              //
//                                                                     //
//Komentar ku kodu sa nachadza nad danym kodom alebo vedla neho        //
//                                                                     //
//---------------------------------------------------------------------//

int main() {
    AVLnode *AVL_root = NULL;
    char search_select, input = ' ';
    int data;

    do {
        printf("\n>Binary search tree (b)\n>Hashing (h)\n>End (e)\n\nSelect: ");
        scanf("\n%c", &search_select);

        switch (search_select) {
            case 'b': //binarne vyhladavacie stromy
                while (input != 'e') {
                    printf("\n>My AVL tree (a)\t>Red-black tree (r)\t>End (e)\nChoose tree: ");
                    scanf("\n%c", &input);

                    switch (input) {
                        case 'a': //moj AVL BVS
                            while (input != 'e') {
                                printf("\n>Insert (i)\t>Search (s)\t>Print (p)\t>End (e)\nChoose action: ");
                                scanf("\n%c", &input);

                                switch (input) {
                                    case 'i': //vlozenie hodnoty do stromu
                                        printf("\nEnter data: ");
                                        scanf("%d", &data);
                                        AVL_root = AVL_insert(AVL_root, NULL, data);
                                        break;

                                    case 's': //vyhladanie hodnoty v strome
                                        break;

                                    case 'p': //vypis jednotlivych uzlov stromu
                                        AVL_print(AVL_root);
                                        printf("\n");
                                        break;

                                    case 'e': //skoncenie s AVL BVS
                                        if (AVL_root != NULL) {
                                            char x = input;
                                            printf("\nDo you want to delete whole tree?\n\tyes (y) / no (n)\n");
                                            scanf("\n%c", &input);
                                            if (input == 'y') {
                                                printf("\tTree was deleted\n");
                                                free(AVL_root);
                                                AVL_root = NULL;
                                            }
                                            input = x;
                                        }
                                        break;

                                    default:
                                        break;
                                }
                            }

                            input = ' ';
                            break;

                        case 'r':
                            while (input != 'e') {
                                printf("\n>Insert (i)\t>Print (p)\t>End (e)\nChoose action: ");
                                scanf("\n%c", &input);

                                switch (input) {
                                    case 'i': //vlozenie hodnoty do stromu
                                        printf("\nEnter data: ");
                                        scanf("%d", &data);
                                        insertion(data);
                                        break;

                                    case 'p': //vypis jednotlivych uzlov stromu
                                        printf("BLACK: 0\t RED: 1\n");
                                        inorderTraversal(root);
                                        printf("\n");
                                        break;

                                    case 'e': //skoncenie s RB BVS
                                        if (root != NULL) {
                                            char x = input;
                                            printf("\nDo you want to delete whole tree?\n\tyes (y) / no (n)\n");
                                            scanf("\n%c", &input);
                                            if (input == 'y') {
                                                printf("\tTree was deleted\n");
                                                free(root);
                                                root = NULL;
                                            }
                                            input = x;
                                        }
                                        break;

                                    default:
                                        break;
                                }
                            }

                            input = ' ';
                            break;

                        default:
                            break;
                    }
                }

                input = ' ';
                break;

            case 'h':
                //hashing things
                break;

            default:
                break;
        }
    } while (search_select != 'e');

    return 0;
}