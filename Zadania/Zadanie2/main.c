#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "my_bvs.c"
#include "my_AVL_tree.c"
#include "red-black_tree.c"

//---------------------------------------------------------------------//
//Poznamka ku komentarom:                                              //
//                                                                     //
//Komentar ku kodu sa nachadza nad danym kodom alebo vedla neho        //
//                                                                     //
//---------------------------------------------------------------------//

BVSnode *BVS_root = NULL; //smernik pre BVS strom bez vyvazenia
AVLnode *AVL_root = NULL; //smernik pre BVS strom s vyvazenim AVL
struct node *RB_root = NULL; //smernik pre cerveno-cierny strom

void BVS_main() {
    char input = ' ';
    int data;

    while (input != 'e') {
        printf("\n>Insert (i)\t>Search (s)\t>Print (p)\t>End (e)\nChoose action: ");
        scanf("\n%c", &input);

        switch (input) {
            case 'i': //vlozenie hodnoty do stromu
                printf("\nEnter data: ");
                scanf("%d", &data);
                BVS_root = BVS_insert(BVS_root, NULL, data);
                break;

            case 's': //vyhladanie hodnoty v strome
                printf("\nEnter searched data: ");
                scanf("%d", &data);

//                clock_t stopwatch;
//                stopwatch = clock();

                BVS_search(BVS_root, data);

//                stopwatch = clock() - stopwatch;
//                double time_taken = ((double)stopwatch) / CLOCKS_PER_SEC; //v sekundach
//                printf("Search for '%d' took %f s to find \n", data, time_taken);
                break;

            case 'p': //vypis jednotlivych uzlov stromu
                BVS_print(BVS_root);
                printf("\n");
                break;

            case 'e': //ukoncenie prace s BVS
                if (BVS_root != NULL) {
                    char x = input;

                    //uzivatel si moze vybrat, ci v strome ponecha hodnoty alebo sa cely premaze
                    printf("\nDo you want to delete whole tree?\n\tyes (y) / no (n)\n");
                    scanf("\n%c", &input);

                    if (input == 'y') {
                        printf("\tTree was deleted\n");
                        free(BVS_root);
                        BVS_root = NULL;
                    }

                    input = x;
                }
                break;

            default:
                break;
        }
    }
}

void AVL_main() {
    char input = ' ';
    int data;

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
                printf("\nEnter searched data: ");
                scanf("%d", &data);
                AVL_search(AVL_root, data);
                break;

            case 'p': //vypis jednotlivych uzlov stromu
                AVL_print(AVL_root);
                printf("\n");
                break;

            case 'e': //ukoncenie prace s AVL BVS
                if (AVL_root != NULL) {
                    char x = input;

                    //uzivatel si moze vybrat, ci v strome ponecha hodnoty alebo sa cely premaze
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
}

void RB_main() {
    char input = ' ';
    int data;

    while (input != 'e') {
        printf("\n>Insert (i)\t>Search (s)\t>Print (p)\t>End (e)\nChoose action: ");
        scanf("\n%c", &input);

        switch (input) {
            case 'i': //vlozenie hodnoty do stromu
                printf("\nEnter data: ");
                scanf("%d", &data);
                insert(&RB_root, data);
                break;

            case 's': //vyhladanie hodnoty v strome
                break;

            case 'p': //vypis jednotlivych uzlov stromu
                printf("BLACK: 0\t RED: 1\n");
                RB_print(RB_root);
                printf("\n");
                break;

            case 'e': //skoncenie s RB BVS
                if (RB_root != NULL) {
                    char x = input;
                    printf("\nDo you want to delete whole tree?\n\tyes (y) / no (n)\n");
                    scanf("\n%c", &input);
                    if (input == 'y') {
                        printf("\tTree was deleted\n");
                        free(RB_root);
                        RB_root = NULL;
                    }
                    input = x;
                }
                break;

            default:
                break;
        }
    }
}

int main() {
    char search_select, input = ' ';
    int data;

    do {
        printf("\n>Binary search tree (b)\n>Hashing (h)\n>End (e)\n\nSelect: ");
        scanf("\n%c", &search_select);

        switch (search_select) {
            case 'b': //binarne vyhladavacie stromy
                printf("\nThis search algorithms are for storing integers.");

                while (input != 'e') {
                    printf("\n>My BVS tree (b)\t>My AVL tree (a)\t>Red-black tree (r)\t>End (e)\nChoose tree: ");
                    scanf("\n%c", &input);

                    switch (input) {
                        case 'b': //moj nevyvazeny BVS
                            BVS_main();
                            break;

                        case 'a': //moj AVL BVS
                            AVL_main();
                            break;

                        case 'r': //prevzaty cerveno-cierny strom
                            RB_main();
                            break;

                        default:
                            break;
                    }
                }

                input = ' ';
                break;

            case 'h':
                printf("\nThis search algorithms are for storing strings.");

                while (input != 'e') {
                    printf("\n>My hash (1)\t>Not my hash (2)\t>End (e)\nChoose tree: ");
                    scanf("\n%c", &input);

                    switch (input) {
                        case '1': //moj hash
                            break;

                        case '2': //nie moj hash
                            break;

                        default:
                            break;
                    }
                }
                break;

            default:
                break;
        }
    } while (search_select != 'e');

    return 0;
}