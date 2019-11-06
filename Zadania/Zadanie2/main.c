#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

#define MUL10 10
#define MAX_NUM_NODES 1000000
#define UPPER 100000
#define LOWER 0

BVSnode *BVS_root = NULL; //smernik pre BVS strom bez vyvazenia
AVLnode *AVL_root = NULL; //smernik pre BVS strom s vyvazenim AVL
struct node *RB_root = NULL; //smernik pre cerveno-cierny strom

void delete_whole_trees() {
    free(BVS_root);
    BVS_root = NULL;
    free(AVL_root);
    AVL_root = NULL;
    free(RB_root);
    RB_root = NULL;
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

void test_trees_search(int num_of_nodes, int *rand_arr) {  // search
    clock_t start, end;
    double exe_time;
    int rand_index = get_random();
    rand_index = rand_index % num_of_nodes;

    printf(".........................Search for '%d'\n", rand_arr[rand_index]);

    //BVS---------------------------------------------------
    start = clock();
    BVS_search(BVS_root, rand_arr[rand_index]);
    end = clock();
    exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("BVS search took %f seconds for %d nodes\n", exe_time, num_of_nodes);
    //BVS---------------------------------------------------

    reset_clock(&start, &end, &exe_time);

    //AVL---------------------------------------------------
    start = clock();
    AVL_search(AVL_root, rand_arr[rand_index]);
    end = clock();
    exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("AVL search took %f seconds for %d nodes\n", exe_time, num_of_nodes);
    //AVL---------------------------------------------------

    reset_clock(&start, &end, &exe_time);

    //RB----------------------------------------------------
    start = clock();
    RB_search(RB_root, rand_arr[rand_index]);
    end = clock();
    exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("RB insert took %f seconds for %d nodes\n\n", exe_time, num_of_nodes);
    //RB----------------------------------------------------

    reset_clock(&start, &end, &exe_time);
}

//priemerny pripad
int test_trees_insert_random() { //test vkladania random cisel
    int num_of_nodes = MUL10;
    int *rand_arr = malloc(num_of_nodes * sizeof(int));

    clock_t start, end;
    double exe_time;

    while (num_of_nodes != MAX_NUM_NODES) {
        for (int j = 0; j < num_of_nodes; j++) {
            rand_arr[j] = get_random();
        }

        printf("//---------------------------------------------------------------------//\n"
               "                            TEST TREE RANDOM                            \n"
               "//---------------------------------------------------------------------//\n");
        printf("//---------------------------------------------------------------------//\n"
               "                                   %d                                 \n"
               "//---------------------------------------------------------------------//\n", num_of_nodes);

        printf("\n.........................Insert.........................\n");

        //BVS---------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            BVS_root = BVS_insert(BVS_root, NULL, rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("BVS insert took %f seconds for %d nodes\n", exe_time, num_of_nodes);
        //BVS---------------------------------------------------

        reset_clock(&start, &end, &exe_time);

        //AVL---------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            AVL_root = AVL_insert(AVL_root, NULL, rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("AVL insert took %f seconds for %d nodes\n", exe_time, num_of_nodes);
        //AVL---------------------------------------------------

        reset_clock(&start, &end, &exe_time);

        //RB----------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            insert(&RB_root, rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("RB insert took %f seconds for %d nodes\n\n", exe_time, num_of_nodes);
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
    int index = 1, last_left = 0, last_right = 0;

    clock_t start, end;
    double exe_time;

    while (num_of_nodes != MAX_NUM_NODES) {
        rand_arr[0] = get_random();

        while (index < num_of_nodes) {
            rand_arr[index] = get_random_range(0, rand_arr[0]-1);
            ++index;

            if (index == num_of_nodes) {
                break;
            }

            rand_arr[index] = get_random_range(rand_arr[0], UPPER);
            ++index;

            if (index == num_of_nodes) {
                break;
            }

        }

        printf("//---------------------------------------------------------------------//\n"
               "                            TEST TREE L-R                            \n"
               "//---------------------------------------------------------------------//\n");
        printf("//---------------------------------------------------------------------//\n"
               "                                   %d                                 \n"
               "//---------------------------------------------------------------------//\n", num_of_nodes);

        printf("\n.........................Insert.........................\n");

        //BVS---------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            BVS_root = BVS_insert(BVS_root, NULL, rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("BVS insert took %f seconds for %d nodes\n", exe_time, num_of_nodes);
        //BVS---------------------------------------------------

        reset_clock(&start, &end, &exe_time);

        //AVL---------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            AVL_root = AVL_insert(AVL_root, NULL, rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("AVL insert took %f seconds for %d nodes\n", exe_time, num_of_nodes);
        //AVL---------------------------------------------------

        reset_clock(&start, &end, &exe_time);

        //RB----------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            insert(&RB_root, rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("RB insert took %f seconds for %d nodes\n\n", exe_time, num_of_nodes);
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

    while (num_of_nodes != MAX_NUM_NODES) {
        int seq_start = get_random();

        for (int j = 0; j < num_of_nodes; j++) {
            rand_arr[j] = seq_start + j;
        }

        printf("//---------------------------------------------------------------------//\n"
               "                            TEST TREE SEQUENCE                            \n"
               "//---------------------------------------------------------------------//\n");
        printf("//---------------------------------------------------------------------//\n"
               "                                   %d                                 \n"
               "//---------------------------------------------------------------------//\n", num_of_nodes);

        printf("\n.........................Insert.........................\n");

        //BVS---------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            BVS_root = BVS_insert(BVS_root, NULL, rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("BVS insert took %f seconds for %d nodes\n", exe_time, num_of_nodes);
        //BVS---------------------------------------------------

        reset_clock(&start, &end, &exe_time);

        //AVL---------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            AVL_root = AVL_insert(AVL_root, NULL, rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("AVL insert took %f seconds for %d nodes\n", exe_time, num_of_nodes);
        //AVL---------------------------------------------------

        reset_clock(&start, &end, &exe_time);

        //RB----------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            insert(&RB_root, rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("RB insert took %f seconds for %d nodes\n\n", exe_time, num_of_nodes);
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



int test_hash_insert_random() { //test vkladania random cisel
    int num_of_nodes = MUL10;
    int *rand_arr = malloc(num_of_nodes * sizeof(int));

    clock_t start, end;
    double exe_time;

    while (num_of_nodes != MAX_NUM_NODES) {
        for (int j = 0; j < num_of_nodes; j++) {
            rand_arr[j] = get_random();
        }

        printf("//---------------------------------------------------------------------//\n"
               "                            TEST HASH RANDOM                            \n"
               "//---------------------------------------------------------------------//\n");
        printf("//---------------------------------------------------------------------//\n"
               "                                   %d                                 \n"
               "//---------------------------------------------------------------------//\n", num_of_nodes);

        printf("\n.........................Insert.........................\n");

        //BVS---------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            BVS_root = BVS_insert(BVS_root, NULL, rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("BVS insert took %f seconds for %d nodes\n", exe_time, num_of_nodes);
        //BVS---------------------------------------------------

        reset_clock(&start, &end, &exe_time);

        //AVL---------------------------------------------------
        start = clock();
        for (int i = 0; i < num_of_nodes; i++) {
            AVL_root = AVL_insert(AVL_root, NULL, rand_arr[i]);
        }
        end = clock();
        exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("AVL insert took %f seconds for %d nodes\n", exe_time, num_of_nodes);
        //AVL---------------------------------------------------

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


int main() {
    srand(time(0));

//    test_trees_insert_random();
//    test_trees_insert_left_right();
//    test_trees_insert_sequence();

    return 0;
}