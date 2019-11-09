#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

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
#define HASH_START_SIZE 11
#define MUL10 10
#define MAX_NUM_NODES 100000
#define UPPER 100000
#define LOWER 0

BVSnode *BVS_root = NULL; //smernik pre BVS strom bez vyvazenia (moja implementacia)
AVLnode *AVL_root = NULL; //smernik pre BVS strom s vyvazenim AVL (moja implementacia)
struct node *RB_root = NULL; //smernik pre cerveno-cierny strom (cudzia implementacia)
struct s_hashmap *NM_hashmap = NULL; //smernik na hash mapu (cudzia implementacia)
HASHMAP *MY_hashmap = NULL; //smernik na hash mapu (moja implementacia)


void delete_whole_trees() {
    free(BVS_root);
    BVS_root = NULL;
    free(AVL_root);
    AVL_root = NULL;
    free(RB_root);
    RB_root = NULL;
    RB_delete_whole();
}

void delete_whole_hashmaps() {
    MY_delete_hashmap(MY_hashmap);
    MY_hashmap = NULL;
    hashmapDelete(NM_hashmap);
    NM_hashmap = NULL;
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

void get_random_char_arr(char **arr, int num_of_nodes) {
    int num, rand_size;
    for (int i = 0; i < num_of_nodes; ++i) {
        rand_size = get_random_range(1, 10);
        for (int j = 0; j < rand_size; ++j) {
            num = rand() % 3;
            if (num == 0) {
                arr[i][j] = (char)('0' + (rand() % 10));
            } else if (num == 1) {
                arr[i][j] = (char)('a' + (rand() % 26));
            } else {
                arr[i][j] = (char)('A' + (rand() % 26));
            }
        }
        arr[i][rand_size] = '\0';
    }
}

int generate_key(char *str) { //suma ascii hodnot stringu, ktore su vynasobene 2^pozicia
    int sum = 0;
    for (int i = 0; i < (int)strlen(str); ++i) {
        sum += (int)pow(2, i+1)*str[i];
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

void test_hash_search(int num_of_nodes, char **rand_arr) {  // search
    int key;
    clock_t start, end;
    double exe_time;
    int rand_index = get_random();
    rand_index = rand_index % num_of_nodes;

    printf("\nSearch   '%s'\n", rand_arr[rand_index]);

    //MY-HASH---------------------------------------------------
    start = clock();
    key = generate_key(rand_arr[rand_index]);
    MY_search(MY_hashmap, rand_arr[rand_index], key);
    end = clock();
    exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("My hash search took %f seconds\n\n", exe_time);
    //MY-HASH---------------------------------------------------

    reset_clock(&start, &end, &exe_time);

    //NOT-MY-HASH-----------------------------------------------
    start = clock();
    key = generate_key(rand_arr[rand_index]);
    hashmapGet(NM_hashmap, key);
    end = clock();
    exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Not my hash search took %f seconds\n", exe_time);
    //NOT-MY-HASH-----------------------------------------------

    reset_clock(&start, &end, &exe_time);
}

int test_hash_insert_random() { //test vkladania random stringov
    int num_of_nodes = MUL10, key;
    char **rand_char_arr;

    rand_char_arr = malloc(num_of_nodes * sizeof(char*));
    for (int i = 0; i < num_of_nodes; i++) {
        rand_char_arr[i] = malloc(MAX_STR);
    }

    clock_t start, end;
    double exe_time;

    while (num_of_nodes <= MAX_NUM_NODES) {
        get_random_char_arr(rand_char_arr, num_of_nodes);
        printf("//---------------------------------------------------------------------//\n"
               "                            TEST HASH RANDOM                            \n"
               "//---------------------------------------------------------------------//\n");
        printf("//---------------------------------------------------------------------//\n"
               "                                   %d                                 \n"
               "//---------------------------------------------------------------------//\n", num_of_nodes);

        NM_hashmap = hashmapCreate(HASH_START_SIZE);
        MY_hashmap = MY_init(HASH_START_SIZE);

        printf("\nHash Insert..............................................\n");

        //MY-HASH---------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            key = generate_key(rand_char_arr[i]);
            MY_hashmap = MY_insert(MY_hashmap, rand_char_arr[i], key);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("My hash insert took %f seconds\n", exe_time);
        //MY-HASH---------------------------------------------------

        reset_clock(&start, &end, &exe_time);

        //NOT-MY-HASH-----------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            key = generate_key(rand_char_arr[i]);
            hashmapInsert(NM_hashmap, rand_char_arr[i], key);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Not my hash insert took %f seconds\n", exe_time);
        //NOT-MY-HASH-----------------------------------------------

        reset_clock(&start, &end, &exe_time);

        //SEARCH-----------------------------------------------
        test_hash_search(num_of_nodes, rand_char_arr);
        //SEARCH-----------------------------------------------

        num_of_nodes *= MUL10;

        free(rand_char_arr);
        rand_char_arr = malloc(num_of_nodes * sizeof(char*));
        for (int i = 0; i < num_of_nodes; i++) {
            rand_char_arr[i] = malloc(MAX_STR);
        }

        delete_whole_hashmaps();
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
                key = generate_key(data);
                hashmapInsert(NM_hashmap, data, key);
                break;

            case 's': //vyhladanie hodnoty v strome
                printf("\nEnter searched data: ");
                fgets(data, MAX_STR, stdin);
                key = generate_key(data);
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
    int key;

    while (input != 'e') {
        char *data = (char*) malloc(MAX_STR);
        printf("\n>Insert (i)\t>Search (s)\t>Print (p)\t>End (e)\nChoose action: ");
        input = (char)getchar();
        getchar();

        switch (input) {
            case 'i': //vlozenie hodnoty do stromu
                printf("\nEnter string: ");
                fgets(data, MAX_STR, stdin);
                strtok(data, "\n");
                key = generate_key(data);
                MY_hashmap = MY_insert(MY_hashmap, data, key);
                break;

            case 's': //vyhladanie hodnoty v strome
                printf("\nEnter searched data: ");
                fgets(data, MAX_STR, stdin);
                strtok(data, "\n");
                key = generate_key(data);
                MY_search(MY_hashmap, data, key);
                break;

            case 'p': //vypis jednotlivych uzlov stromu
                MY_print(MY_hashmap);
                printf("\n");
                break;

            case 'e': //ukoncenie prace s BVS
                if (MY_hashmap != NULL) {
                    char x = input;

                    //uzivatel si moze vybrat, ci v strome ponecha hodnoty alebo sa cely premaze
                    printf("\nDo you want to delete whole tree?\n\tyes (y) / no (n)\n");
                    scanf("\n%c", &input);

                    if (input == 'y') {
                        printf("\tHash was deleted\n");
                        MY_delete_hashmap(MY_hashmap);
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

    test_hash_insert_random();
//    test_trees_insert_random();
//    test_trees_insert_left_right();
//    test_trees_insert_sequence();

    return 0;
}