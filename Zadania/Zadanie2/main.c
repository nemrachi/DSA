#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//files with trees
#include "my_bvs.c"
#include "my_AVL_tree.c"
#include "red-black_tree.c"

//files with hash
#include "my_hash.c"
#include "not_my_hash.c"

//---------------------------------------------------------------------//
//Poznamka ku komentarom:                                              //
//                                                                     //
//Komentar ku kodu sa nachadza nad danym kodom alebo vedla neho        //
//                                                                     //
//---------------------------------------------------------------------//

#define MAX_STR 100
#define HASH_START_SIZE 7
#define MUL10 10
#define MAX_NUM_NODES 100000
#define UPPER 100000
#define LOWER 0

BVSnode *BVS_root = NULL; //smernik pre BVS strom bez vyvazenia (moja implementacia)
AVLnode *AVL_root = NULL; //smernik pre BVS strom s vyvazenim AVL (moja implementacia)
struct node *RB_root = NULL; //smernik pre cerveno-cierny strom (cudzia implementacia)
struct s_hashmap *NM_hashmap = NULL; //smernik na hash mapu (cudzia implementacia)
HASH_TABLE *MY_hash_table = NULL; //smernik na hash mapu (moja implementacia)


void delete_whole_trees() {
    free(BVS_root);
    BVS_root = NULL;
    free(AVL_root);
    AVL_root = NULL;
    free(RB_root);
    RB_root = NULL;
    RB_delete_whole();
}

void reset_clock(clock_t *start, clock_t *end, double *exe_time) {
    *start = (clock_t) NULL;
    *end = (clock_t) NULL;
    *exe_time = 0;
}

int get_random() {
    return (rand() % (UPPER + 1 - LOWER)) + LOWER;
}

int get_random_range(int lower, int upper) {
    return (rand() % (upper + 1 - lower)) + lower;
}

int ascii_sum(char *str) { //suma ascii hodnot stringu
    int sum = 0;
    for (int i = 0; i < (int)strlen(str); ++i) {
        sum += str[i];
    }
    return sum;
}

void fill_arr_random(int *arr, int num) {
    for (int j = 0; j < num; j++) {
        arr[j] = get_random();
    }
}

void fill_arr_seq(int *arr, int num) {
    for (int j = 0; j < num; j++) {
        arr[j] = j;
    }
}

void fill_arr_left_right(int *arr, int num) {
    int index = 1;
    arr[0] = get_random();

    while (index < num) {
        arr[index] = get_random_range(0, arr[0]-1);
        ++index;

        if (index == num) {
            break;
        }

        arr[index] = get_random_range(arr[0], UPPER);
        ++index;

        if (index == num) {
            break;
        }

    }
}

void test_trees_search(int num_of_nodes, int *rand_arr) {  // search
    clock_t start, end;
    double exe_time;
    int rand_index = get_random();
    rand_index = rand_index % num_of_nodes;

    printf("Search   '%d'\n", rand_arr[rand_index]);

    //BVS---------------------------------------------------
    start = clock();
    BVS_search(BVS_root, rand_arr[rand_index]);
    end = clock();
    exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("BVS search took %f seconds\n", exe_time);
    //BVS---------------------------------------------------

    reset_clock(&start, &end, &exe_time);

    //AVL---------------------------------------------------
    start = clock();
    AVL_search(AVL_root, rand_arr[rand_index]);
    end = clock();
    exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("AVL search took %f seconds\n", exe_time);
    //AVL---------------------------------------------------

    reset_clock(&start, &end, &exe_time);

    //RB----------------------------------------------------
    start = clock();
    RB_search(RB_root, rand_arr[rand_index]);
    end = clock();
    exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("RB insert took %f seconds\n\n", exe_time);
    //RB----------------------------------------------------

    reset_clock(&start, &end, &exe_time);
}

//priemerny pripad
int test_trees_insert_random() { //test vkladania random cisel
    int num_of_nodes = MUL10;
    int *rand_arr = malloc(num_of_nodes * sizeof(int));

    clock_t start, end;
    double exe_time;

    while (num_of_nodes <= MAX_NUM_NODES) {
        fill_arr_random(rand_arr, num_of_nodes);

        printf("//---------------------------------------------------------------------//\n"
               "                            TEST TREE RANDOM                            \n"
               "//---------------------------------------------------------------------//\n");
        printf("//---------------------------------------------------------------------//\n"
               "                                   %d                                 \n"
               "//---------------------------------------------------------------------//\n", num_of_nodes);

        printf("\nTree Insert..............................................\n\n");

        //BVS---------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            BVS_root = BVS_insert(BVS_root, rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("BVS insert took %f seconds\n", exe_time);
        //BVS---------------------------------------------------

        reset_clock(&start, &end, &exe_time);

        //AVL---------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            AVL_root = AVL_insert(AVL_root, NULL, rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("AVL insert took %f seconds\n", exe_time);
        //AVL---------------------------------------------------

        reset_clock(&start, &end, &exe_time);

        //RB----------------------------------------------------
        RB_set_root();

        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            red_black_insert(rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("RB insert took %f seconds\n\n", exe_time);

        RB_root = RB_get_root();
        //RB----------------------------------------------------

        reset_clock(&start, &end, &exe_time);

        //SEARCH-----------------------------------------------
            test_trees_search(num_of_nodes, rand_arr);
        //SEARCH-----------------------------------------------

        num_of_nodes *= MUL10;
        rand_arr = realloc(rand_arr, num_of_nodes * sizeof(int));

        delete_whole_trees();
    }

    return 0;
}

//optimalny pripad
int test_trees_insert_left_right() { //test striedaveho vkladania dolava a doprava
    int num_of_nodes = MUL10;
    int *rand_arr = malloc(num_of_nodes * sizeof(int));

    clock_t start, end;
    double exe_time;

    while (num_of_nodes <= MAX_NUM_NODES) {
        fill_arr_left_right(rand_arr, num_of_nodes);

        printf("//---------------------------------------------------------------------//\n"
               "                            TEST TREE L-R                            \n"
               "//---------------------------------------------------------------------//\n");
        printf("//---------------------------------------------------------------------//\n"
               "                                   %d                                 \n"
               "//---------------------------------------------------------------------//\n", num_of_nodes);

        printf("\nTree Insert..............................................\n\n");

        //BVS---------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            BVS_root = BVS_insert(BVS_root, rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("BVS insert took %f seconds\n", exe_time);
        //BVS---------------------------------------------------

        reset_clock(&start, &end, &exe_time);

        //AVL---------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            AVL_root = AVL_insert(AVL_root, NULL, rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("AVL insert took %f seconds\n", exe_time);
        //AVL---------------------------------------------------

        reset_clock(&start, &end, &exe_time);

        //RB----------------------------------------------------
        RB_set_root();

        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            red_black_insert(rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("RB insert took %f seconds\n\n", exe_time);

        RB_root = RB_get_root();
        //RB----------------------------------------------------

        reset_clock(&start, &end, &exe_time);

        //SEARCH-----------------------------------------------
        test_trees_search(num_of_nodes, rand_arr);
        //SEARCH-----------------------------------------------

        num_of_nodes *= MUL10;
        rand_arr = realloc(rand_arr, num_of_nodes * sizeof(int));

        delete_whole_trees();
    }

    return 0;
}

//najhorsi pripad
int test_trees_insert_sequence() { //test vkladania random cisel
    int num_of_nodes = MUL10;
    int *rand_arr = malloc(num_of_nodes * sizeof(int));

    clock_t start, end;
    double exe_time;

    while (num_of_nodes <= MAX_NUM_NODES) {
        fill_arr_seq(rand_arr, num_of_nodes);

        printf("//---------------------------------------------------------------------//\n"
               "                            TEST TREE SEQUENCE                            \n"
               "//---------------------------------------------------------------------//\n");
        printf("//---------------------------------------------------------------------//\n"
               "                                   %d                                 \n"
               "//---------------------------------------------------------------------//\n", num_of_nodes);

        printf("\nTree Insert..............................................\nvvv\n");

        //BVS---------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            BVS_root = BVS_insert(BVS_root, rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("BVS insert took %f seconds\n", exe_time);
        //BVS---------------------------------------------------

        reset_clock(&start, &end, &exe_time);

        //AVL---------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            AVL_root = AVL_insert(AVL_root, NULL, rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("AVL insert took %f seconds\n", exe_time);
        //AVL---------------------------------------------------

        reset_clock(&start, &end, &exe_time);

        //RB----------------------------------------------------
        RB_set_root();

        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            red_black_insert(rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("RB insert took %f seconds\n\n", exe_time);

        RB_root = RB_get_root();
        //RB----------------------------------------------------

        reset_clock(&start, &end, &exe_time);

        //SEARCH-----------------------------------------------
        test_trees_search(num_of_nodes, rand_arr);
        //SEARCH-----------------------------------------------

        num_of_nodes *= MUL10;
        rand_arr = realloc(rand_arr, num_of_nodes * sizeof(int));

        delete_whole_trees();
    }

    return 0;
}

int test_hash_insert_same(int *rand_arr) { //test vkladania random cisel
    int num_of_nodes = MUL10;

    clock_t start, end;
    double exe_time;

    while (num_of_nodes <= MAX_NUM_NODES) {
        printf("//---------------------------------------------------------------------//\n"
               "                            TEST HASH RANDOM                            \n"
               "//---------------------------------------------------------------------//\n");
        printf("//---------------------------------------------------------------------//\n"
               "                                   %d                                 \n"
               "//---------------------------------------------------------------------//\n", num_of_nodes);

        printf("\nHash Insert..............................................\n\n");

        //BVS---------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            BVS_root = BVS_insert(BVS_root, rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("BVS insert took %f seconds\n", exe_time);
        //BVS---------------------------------------------------

        reset_clock(&start, &end, &exe_time);

        //AVL---------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            AVL_root = AVL_insert(AVL_root, NULL, rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("AVL insert took %f seconds\n", exe_time);
        //AVL---------------------------------------------------

        reset_clock(&start, &end, &exe_time);

        //SEARCH-----------------------------------------------

        //SEARCH-----------------------------------------------

        num_of_nodes *= MUL10;
        rand_arr = realloc(rand_arr, num_of_nodes * sizeof(int));

        delete_whole_trees();
    }

    return 0;
}


void not_my_hash_main() {
    char input = ' ';
    char data[MAX_STR];
    int key;

    while (input != 'e') {
        printf("\n>Insert (i)\t>Search (s)\t>Print (p)\t>End (e)\nChoose action: ");
        input = (char)getchar();
        getchar();

        switch (input) {
            case 'i': //vlozenie hodnoty do stromu
                printf("\nEnter string: ");
                fgets(data, MAX_STR, stdin);
                strtok(data, "\n");
                key = ascii_sum(data);
                hashmapInsert(NM_hashmap, data, key);
                break;

            case 's': //vyhladanie hodnoty v strome
                printf("\nEnter searched data: ");
                fgets(data, MAX_STR, stdin);
                key = ascii_sum(data);
                char *hm_data = hashmapGet(NM_hashmap, key);
                if (hm_data != NULL) {
                    printf("searched %s", hm_data);
                } else {
                    printf("find nothing");
                }
                break;

            case 'p': //vypis jednotlivych uzlov stromu
                NM_print(NM_hashmap);
                printf("\n");
                break;

            case 'e': //ukoncenie prace s BVS
                if (NM_hashmap != NULL) {
                    char x = input;

                    //uzivatel si moze vybrat, ci v strome ponecha hodnoty alebo sa cely premaze
                    printf("\nDo you want to delete whole tree?\n\tyes (y) / no (n)\n");
                    scanf("\n%c", &input);

                    if (input == 'y') {
                        printf("\tHash was deleted\n");
                        hashmapDelete(NM_hashmap);
                        NM_hashmap = NULL;
                    }

                    input = x;
                }
                break;

            default:
                break;
        }
    }
}

void my_hash_main() {
    char input = ' ';
    char data[MAX_STR];
    int key;

    while (input != 'e') {
        printf("\n>Insert (i)\t>Search (s)\t>Print (p)\t>End (e)\nChoose action: ");
        input = (char)getchar();
        getchar();

        switch (input) {
            case 'i': //vlozenie hodnoty do stromu
                printf("\nEnter string: ");
                fgets(data, MAX_STR, stdin);
                strtok(data, "\n");
                key = ascii_sum(data);
                MY_hash_table = MY_insert(MY_hash_table, data, key);
                break;

            case 's': //vyhladanie hodnoty v strome
                printf("\nEnter searched data: ");
                fgets(data, MAX_STR, stdin);
                key = ascii_sum(data);
                char *hm_data = hashmapGet(NM_hashmap, key);
                if (hm_data != NULL) {
                    printf("searched %s", hm_data);
                } else {
                    printf("find nothing");
                }
                break;

            case 'p': //vypis jednotlivych uzlov stromu
                MY_print(MY_hash_table);
                printf("\n");
                break;

            case 'e': //ukoncenie prace s BVS
                if (MY_hash_table != NULL) {
                    char x = input;

                    //uzivatel si moze vybrat, ci v strome ponecha hodnoty alebo sa cely premaze
                    printf("\nDo you want to delete whole tree?\n\tyes (y) / no (n)\n");
                    scanf("\n%c", &input);

                    if (input == 'y') {
                        printf("\tHash was deleted\n");
                        MY_delete_hashmap(MY_hash_table);
                        NM_hashmap = NULL;
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
    srand(time(0));

    NM_hashmap = hashmapCreate(HASH_START_SIZE);
    MY_hash_table = MY_init(HASH_START_SIZE);


    //not_my_hash_main();
    my_hash_main();
//    test_trees_insert_random();
//    test_trees_insert_left_right();
//    test_trees_insert_sequence();

    return 0;
}