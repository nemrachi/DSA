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

//---------------------------------------------------------------------//
//Konstanty                                                            //
//---------------------------------------------------------------------//
#define MAX_STR 10 //maximalna velkost stringu vkladana do hashovacej tabulky
#define HASH_START_SIZE 11 //zaciatocna velkost tabulky
#define MAX_ELEMENTS 100000 //maximalny pocet vkladanych prvkov do tabulky
#define UPPER 100000
#define LOWER 0

//---------------------------------------------------------------------//
//Smerniky na struktury jednotlivych algoritmov                        //
//---------------------------------------------------------------------//
BVSnode *BVS_root = NULL; //smernik na BVS strom bez vyvazenia (moja implementacia)
AVLnode *AVL_root = NULL; //smernik na BVS strom s vyvazovacim algoritmom AVL (moja implementacia)
struct node *RB_root = NULL; //smernik na cerveno-cierny strom (cudzia implementacia)
struct s_hashmap *NM_hashmap = NULL; //smernik na hashovaciu tabulku (cudzia implementacia)
HASHMAP *MY_hash_tab = NULL; //smernik na hashovacou tabulku (moja implementacia)

//---------------------------------------------------------------------//
//Pomocne funkcie                                                      //
//---------------------------------------------------------------------//
void delete_whole_trees() {
    free(BVS_root);
    BVS_root = NULL;
    free(AVL_root);
    AVL_root = NULL;
    free(RB_root);
    RB_root = NULL;
    RB_delete_whole();
}

void delete_whole_hash_tables() {
    MY_delete_hashmap(MY_hash_tab);
    MY_hash_tab = NULL;
    hashmapDelete(NM_hashmap);
    NM_hashmap = NULL;
}

void reset_clock(clock_t *start, clock_t *end, double *exe_time) {
    *start = (clock_t) NULL;
    *end = (clock_t) NULL;
    *exe_time = 0;
}

//funkcia vrati nahodne cisla z rozmedzia urceneho konstantami
int get_random() {
    return (rand() % (UPPER + 1 - LOWER)) + LOWER;
}

//funkcia vrati nahodne cisla z rozmedzia urceneho pouzivatelom
int get_random_range(int lower, int upper) {
    return (rand() % (upper + 1 - lower)) + lower;
}

//funkcia vrati pole nahodnych stringov nahodnej dlzky
void get_random_char_arr(char **arr, int num_of_nodes) {
    int option, rand_size;
    for (int i = 0; i < num_of_nodes; ++i) {
        rand_size = get_random_range(2, MAX_STR);
        for (int j = 0; j < (rand_size-1); j++) {
            option = rand() % 3;
            if (option == 0) {
                arr[i][j] = (char)('0' + (rand() % 10));
            } else if (option == 1) {
                arr[i][j] = (char)('a' + (rand() % 26));
            } else {
                arr[i][j] = (char)('A' + (rand() % 26));
            }
        }
        arr[i][rand_size-1] = '\0';
    }
}

//funkcia vrati pole nahodnych cisel
void get_arr_random(int *arr, int num) {
    for (int j = 0; j < num; j++) {
        arr[j] = get_random();
    }
}

//funkcia vrati pole, ktore obsahuje postupnost cisel
void get_arr_seq(int *arr, int num) {
    for (int j = 0; j < num; j++) {
        arr[j] = j;
    }
}

//funkcia vrati pole, ktore obsahuje pole, v ktorom sa na prvom mieste nachadza cislo korena a potom na striedacku
//su ulozene cisla mensie/vacsie od korenoveho cisla
void get_arr_left_right(int *arr, int num) {
    int index = 1;
    arr[0] = get_random();

    while (index < num) {
        arr[index] = get_random_range(0, arr[0]-1); //mensie od korenoveho cisla
        ++index;

        if (index == num) {
            break;
        }

        arr[index] = get_random_range(arr[0], UPPER); //vacsie od korenoveho cisla
        ++index;
    }
}

//funkcia na generovanie kluca - suma ascii hodnot pismen zo stringu, ktore su vynasobene 2^pozicia
int generate_key(char *str) {
    int sum = 0, len = (int)strlen(str);
    for (int i = 0; i < len; i++) {
        sum += ((int)pow(2, i+1) * (int)str[i]);
    }
    return sum;
}

//---------------------------------------------------------------------//
//TESTY                                                                //
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
//STROMY                                                               //
//---------------------------------------------------------------------//

//TEST - prehladavanie stromov
void test_trees_search(int *rand_arr, int num_of_nodes) {
    clock_t start, end;
    double exe_time;
    int rand_index = get_random() % num_of_nodes;

    printf("..............................................Search for\t%d\n", rand_arr[rand_index]);

    //BVS---------------------------------------------------
    start = clock(); //"pustenie" stopiek
    BVS_search(BVS_root, rand_arr[rand_index]);
    end = clock(); //"zastavenie" stopiek
    exe_time = ((double) (end - start)) / CLOCKS_PER_SEC; //vypocet casu vykonania funkcie v sekundach
    printf("BVS search took %f seconds\n", exe_time);
    //BVS---------------------------------------------------

    reset_clock(&start, &end, &exe_time); //resetovanie stopiek

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

//TEST - vkladanie do stromu - priemerny pripad (nahodne cisla)
int test_trees_insert_random() {
    clock_t start, end;
    double exe_time;
    int num_of_nodes = 10; //zacina vkladanim 10 prvkov
    int *rand_arr = malloc(num_of_nodes * sizeof(int));

    while (num_of_nodes <= MAX_ELEMENTS) { //maximalne vlozi 100 000 prvkov (iteruje po nasobkoch 10)
        get_arr_random(rand_arr, num_of_nodes); //vygeneruje nahodne cisla

        printf("//---------------------------------------------------------------------//\n"
               "                            TEST TREE RANDOM                            \n"
               "//---------------------------------------------------------------------//\n");
        printf("//---------------------------------------------------------------------//\n"
               "                                   %d                                 \n"
               "//---------------------------------------------------------------------//\n", num_of_nodes);

        printf("\nTree Insert..............................................\n\n");

        //BVS---------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) { //vkladanie n prvkov
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
        RB_set_root(); //inicializovanie stromovej struktury

        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            red_black_insert(rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("RB insert took %f seconds\n\n", exe_time);

        RB_root = RB_get_root(); //ulozenie stromovej struktury
        //RB----------------------------------------------------

        reset_clock(&start, &end, &exe_time);

        //SEARCH-----------------------------------------------
            test_trees_search(rand_arr, num_of_nodes);
        //SEARCH-----------------------------------------------

        num_of_nodes *= 10;
        rand_arr = realloc(rand_arr, num_of_nodes * sizeof(int));
        delete_whole_trees();
    }

    return 0;
}

//TEST - vkladanie do stromu - optimalny pripad (nahodne cisla vkladane na "striedacku")
int test_trees_insert_left_right() {
    clock_t start, end;
    double exe_time;
    int num_of_nodes = 10;
    int *rand_arr = malloc(num_of_nodes * sizeof(int));

    while (num_of_nodes <= MAX_ELEMENTS) {
        get_arr_left_right(rand_arr, num_of_nodes);

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
        test_trees_search(rand_arr, num_of_nodes);
        //SEARCH-----------------------------------------------

        num_of_nodes *= 10;
        rand_arr = realloc(rand_arr, num_of_nodes * sizeof(int));
        delete_whole_trees();
    }

    return 0;
}

//TEST - vkladanie do stromu - najhorsi pripad (postupnost cisel)
int test_trees_insert_sequence() {
    clock_t start, end;
    double exe_time;
    int num_of_nodes = 10;
    int *rand_arr = malloc(num_of_nodes * sizeof(int));

    while (num_of_nodes <= MAX_ELEMENTS) {
        get_arr_seq(rand_arr, num_of_nodes);

        printf("//---------------------------------------------------------------------//\n"
               "                            TEST TREE SEQUENCE                            \n"
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
        test_trees_search(rand_arr, num_of_nodes);
        //SEARCH-----------------------------------------------

        num_of_nodes *= 10;
        rand_arr = realloc(rand_arr, num_of_nodes * sizeof(int));
        delete_whole_trees();
    }

    return 0;
}

//---------------------------------------------------------------------//
//TESTY                                                                //
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
//HASHOVANIE                                                           //
//---------------------------------------------------------------------//

//TEST - prehladavanie hashovacich tabuliek
void test_hash_search(int num_of_nodes, char **rand_arr) {
    clock_t start, end;
    double exe_time;
    int rand_index = get_random() % num_of_nodes, key;

    printf("\n..............................................Search for\t%s\n", rand_arr[rand_index]);

    //MY-HASH---------------------------------------------------
    start = clock();
    key = generate_key(rand_arr[rand_index]);
    MY_search(MY_hash_tab, rand_arr[rand_index], key);
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

//TEST - vkladanie do hashovacej tabulky - nahodne stringy
int test_hash_insert_random() {
    clock_t start, end;
    double exe_time;
    int key, num_of_nodes = 10; //zacina vkladanim 10 prvkov
    char **rand_char_arr;

    while (num_of_nodes <= MAX_ELEMENTS) { //maximalne vlozi 100 000 prvkov (iteruje po nasobkoch 10)
        rand_char_arr = malloc(num_of_nodes * sizeof(char*));
        for (int i = 0; i < num_of_nodes; i++) {
            rand_char_arr[i] = malloc(MAX_STR);
        }
        get_random_char_arr(rand_char_arr, num_of_nodes); //vygeneruje nahodne stringy

        printf("//---------------------------------------------------------------------//\n"
               "                            TEST HASH RANDOM                            \n"
               "//---------------------------------------------------------------------//\n");
        printf("//---------------------------------------------------------------------//\n"
               "                                   %d                                 \n"
               "//---------------------------------------------------------------------//\n", num_of_nodes);

        NM_hashmap = hashmapCreate(HASH_START_SIZE); //inicializacia cudzej hash tabulky
        MY_hash_tab = MY_init(HASH_START_SIZE); //inicializacia mojej hash tabulky

        printf("\nHashing Insert..............................................\n\n");

        //MY-HASH---------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            key = generate_key(rand_char_arr[i]);
            MY_hash_tab = MY_insert(MY_hash_tab, rand_char_arr[i], key);
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

        for (int i = 0; i < num_of_nodes; i++) {
            free(rand_char_arr[i]);
        }
        free(rand_char_arr);

        num_of_nodes *= 10;
        delete_whole_hash_tables();
    }
    return 0;
}

int main() {
    srand(time(0));

    test_hash_insert_random();
//    test_trees_insert_random();
//    test_trees_insert_left_right();
//    test_trees_insert_sequence();

    return 0;
}