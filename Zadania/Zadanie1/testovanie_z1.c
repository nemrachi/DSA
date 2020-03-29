#include <stdlib.h>
#include <time.h>
#include "zadanie1.c"

int get_random_range(int low, int up) {  
    return (rand() % (up - low + 1)) + low;
} 

void print_results(int i, int alloc_block_size, int taken_by_user, int taken, int region_size) {
    printf("Itterations: %d\n", i);
    printf("Alloc block size: %d\n", alloc_block_size);
    float perc_taken = ((float)taken_by_user / (float)region_size) * 100.0;
    printf("Taken by user: %d/%d ->  %.02f%%\n", taken_by_user, region_size, perc_taken);
    perc_taken = ((float)taken / (float)region_size) * 100.0;
    printf("Taken memory: %d/%d ->  %.02f%%\n", taken, region_size, perc_taken);
    perc_taken = ((float)taken_by_user / (float)taken) * 100.0;
    printf("Taken usage: %d/%d ->  %.02f%%\n", taken_by_user, taken, perc_taken);
}

void free_all(void ** array_p, int i) {
    for (int k = 0; k <= i; k++) {
        //printf("%d: %p\n", k, array_p[k]);
        if (memory_free(array_p[k])) {
            if (k < i) {
                printf("ERROR\n\n");
                return;
            }
        }
    }
    printf("-----FREE TEST ENDED-----\n\n");
}

//---------------------------------------------------------------------//
//                                                                     //
//       Test same free blocks                                         //
//                                                                     //
//---------------------------------------------------------------------//
//prideľovanie rovnakých blokov malej veľkosti (veľkosti 8 až 24 bytov)
void test_alloc_same_free_blocks(int region_size) {
    char region[region_size]; //blok pamate, na ktorom bude realizovana alokacia
    memory_init(region, region_size); //inicializacia volneho bloku pamate

    int alloc_block_size = get_random_range(8, 24);

    void * array_p[region_size];

    for (int i = 0; i < region_size; i++)
    {
        array_p[i] = memory_alloc(alloc_block_size);
        //printf("%d: %p\n", i, array_p[i]);

        if (array_p[i] == NULL) {
            printf("\n-----MALLOC TEST ENDED-----\n");
            
            int taken_by_user = (i * (alloc_block_size));
            int taken = (i * (alloc_block_size + (int)ALLOC_HEAD_OFF)) + (int)MEM_HEAD_OFF;
            print_results(i, alloc_block_size, taken_by_user, taken, region_size);

            free_all(array_p, i);

            break;
        }
    }
}


//---------------------------------------------------------------------//
//                                                                     //
//       Test differen free blocks                                     //
//                                                                     //
//---------------------------------------------------------------------//
//  prideľovanie nerovnakých blokov malej veľkosti (náhodné veľkosti 8 až 24 bytov)
void test_alloc_different_free_blocks_in_range_8_to_24(int region_size) {
    char region[region_size]; //blok pamate, na ktorom bude realizovana alokacia
    memory_init(region, region_size); //inicializacia volneho bloku pamate

    int alloc_block_size, sum = 0;

    void * array_p[region_size];

    for (int i = 0; i < region_size; i++)
    {
        alloc_block_size = get_random_range(8, 24);
        array_p[i] = memory_alloc(alloc_block_size);

        if (array_p[i] != NULL) {
            printf("%d\t", alloc_block_size);
            if ((i != 0) && ((i % 5) == 0)) {
                printf("\n");
            }
            sum += alloc_block_size;
        } else {
            printf("\n-----MALLOC TEST ENDED-----\n");
            
            int taken_by_user = sum;
            int taken = sum + (i * (int)ALLOC_HEAD_OFF) + (int)MEM_HEAD_OFF;
            print_results(i, 0, taken_by_user, taken, region_size);

            free_all(array_p, i);

            break;
        }
    }
}

//  prideľovanie nerovnakých blokov väčšej veľkosti (veľkosti 500 až 5000 bytov)
void test_alloc_different_free_blocks_in_range_500_to_5000(int region_size) {
    char region[region_size]; //blok pamate, na ktorom bude realizovana alokacia
    memory_init(region, region_size); //inicializacia volneho bloku pamate

    int alloc_block_size, sum = 0;

    void * array_p[region_size];

    for (int i = 0; i < region_size; i++)
    {
        alloc_block_size = get_random_range(500, region_size);
        array_p[i] = memory_alloc(alloc_block_size);

        if (array_p[i] != NULL) {
            printf("%d\t", alloc_block_size);
            if ((i != 0) && ((i % 5) == 0)) {
                printf("\n");
            }
            sum += alloc_block_size;
        } else {
            printf("\n-----MALLOC TEST ENDED-----\n");
            
            int taken_by_user = sum;
            int taken = sum + (i * (int)ALLOC_HEAD_OFF) + (int)MEM_HEAD_OFF;
            print_results(i, 0, taken_by_user, taken, region_size);

            free_all(array_p, i);

            break;
        }
    }
}

//  prideľovanie nerovnakých blokov malých a veľkých veľkostí (veľkosti od 8 bytov do 50 000)
void test_alloc_different_free_blocks_in_range_8_to_50000(int region_size) {
    char region[region_size]; //blok pamate, na ktorom bude realizovana alokacia
    memory_init(region, region_size); //inicializacia volneho bloku pamate

    int alloc_block_size, sum = 0;

    void * array_p[region_size];

    for (int i = 0; i < region_size; i++)
    {
        alloc_block_size = get_random_range(8, region_size);
        array_p[i] = memory_alloc(alloc_block_size);

        if (array_p[i] != NULL) {
            printf("%d\t", alloc_block_size);
            if ((i != 0) && ((i % 5) == 0)) {
                printf("\n");
            }
            sum += alloc_block_size;
        } else {
            printf("\n-----MALLOC TEST ENDED-----\n");
            
            int taken_by_user = sum;
            int taken = sum + (i * (int)ALLOC_HEAD_OFF) + (int)MEM_HEAD_OFF;
            print_results(i, 0, taken_by_user, taken, region_size);
            
            free_all(array_p, i);

            break;
        }
    }
}

//---------------------------------------------------------------------//
//                                                                     //
//       Test free function                                            //
//                                                                     //
//---------------------------------------------------------------------//
void test_connecting_free_block() {
    printf("test5\n");
    printf("-----------------------------------test_connecting_free_block\n");
    int region_size = 100;
    char region[region_size]; //blok pamate, na ktorom bude realizovana alokacia
    memory_init(region, region_size); //inicializacia volneho bloku pamate

    void * p1 = NULL, * p2 = NULL, * p3 = NULL;
    BLOCK_HEAD * free_block = NULL;
    int alloc_block_size, sum = 0;

    //alokovanie
    p1 = memory_alloc(10);
    p2 = memory_alloc(10);
    p3 = memory_alloc(10);

    int taken_by_user = 40;
    int taken = 40 + (3 * (int)ALLOC_HEAD_OFF) + (int)MEM_HEAD_OFF;

    printf("\n-----MALLOC TEST ENDED-----\n");
    print_results(3, 10, taken_by_user, taken, region_size);

    free_block = (BLOCK_HEAD *)((char *)memory_head + memory_head->first_free_offset); //smernik na prvy volny blok v pamati
    printf("\nfirst free memory block address: %p\n", (char *)free_block);

    //uvolnovanie
    //stred
    if (memory_free(p2)) {
        printf("ERROR\n\n");
    } else {
        printf("\n-----FREE TEST ENDED-----\n\n");
        printf("p2 is free\n");
    }

    free_block = (BLOCK_HEAD *)((char *)memory_head + memory_head->first_free_offset); //smernik na prvy volny blok v pamati

    if ((char *)free_block == ((char *)p2 - (int)ALLOC_HEAD_OFF)) { //ak sa smernik na prvy volny blok v pamati rovna adresou smerniku p2...
                                                                    //...znamena to, ze p2 sa uvolnil a bol evidovany ako prvy volny blok
        printf("first free memory block address: %p\n", (char *)free_block);
        printf("first free memory block size and offset to next block: %d %d\n", free_block->block_size, free_block->next_block_off);
    }
    
    //uvolnovanie
    //prvy
    if (memory_free(p1)) {
        printf("ERROR\n\n");
    } else {
        printf("\n-----FREE TEST ENDED-----\n\n");
        printf("p1 is free\n");
    }

    free_block = (BLOCK_HEAD *)((char *)memory_head + memory_head->first_free_offset);//smernik na prvy volny blok v pamati

    if ((char *)free_block == ((char *)p1 - (int)ALLOC_HEAD_OFF)) { //ak sa smernik na prvy volny blok v pamati rovna adresou smerniku p1...
                                                                    //...znamena to, ze p2 sa uvolnil a bol evidovany ako prvy volny blok
        printf("first free memory block address: %p\n", (char *)free_block);
        printf("first free memory block size and offset to next block: %d %d\n", free_block->block_size, free_block->next_block_off);
        //velkost prveho volneho bloku sedi velkosti uvolnenych 2 alokovanych blokov s velkostou 10 a velkostou hlavicky 4B
    }

    //uvolnovanie
    //posledny
    if (memory_free(p3)) {
        printf("ERROR\n\n");
    } else {
        printf("\n-----FREE TEST ENDED-----\n\n");
        printf("p3 is free\n");
    }

    free_block = (BLOCK_HEAD *)((char *)memory_head + memory_head->first_free_offset); //smernik na prvy volny blok v pamati

    if ((char *)free_block == ((char *)p3 - (int)ALLOC_HEAD_OFF)) { //ak sa smernik na prvy volny blok v pamati rovna adresou smerniku p3...
                                                                    //...nemoze sa to stat, lebo uvolnena pamat p3 bola spojena s p2 a p1...
        printf("This will not be printed.\n");
        printf("first free memory block address: %p\n", (char *)free_block);
        printf("first free memory block size and offset to next block: %d %d\n", free_block->block_size, free_block->next_block_off);
    }

    if ((char *)free_block == ((char *)p1 - (int)ALLOC_HEAD_OFF)) { //...takze p1 musimat rovnaku adresu a smernik na prvy volny blok
        printf("This will be printed.\n");
        printf("first free memory block address: %p\n", (char *)free_block);
        printf("first free memory block size and offset to next block: %d %d\n", free_block->block_size, free_block->next_block_off);
        //velkost prveho volneho bloku sedi velkosti uvolnenych 3 alokovanych blokov s velkostou 10 a velkostou hlavicky 4B a to je spojene zo zvyskom volnej pamate
        //cize sme sa dostali vlastne do prvotneho stavu pamate pred vsetkymi alokovaniami
        //100 - 8B hlavicka pamate (memory_head) = 92
    }
}




//---------------------------------------------------------------------//
//                                                                     //
//       MAIN                                                          //
//                                                                     //
//---------------------------------------------------------------------//

void test1() {
    int region_size;

    printf("test1\n");
    printf("-----------------------------------test same free block\n");
    printf("pri pouziti malych celkoych blokov pre spravcu pamate (do 50 bytov, do 100 bytov, do 200 bytov)\n\n");
    region_size = 50;
    test_alloc_same_free_blocks(region_size);
    region_size = 100;
    test_alloc_same_free_blocks(region_size);
    region_size = 200;
    test_alloc_same_free_blocks(region_size);
}

void test2() {
    int region_size;

    printf("test2\n");
    printf("-----------------------------------test different free block\n");
    printf("pri pouziti malych celkoych blokov pre spravcu pamate (do 50 bytov, do 100 bytov, do 200 bytov)\n\n");
    region_size = 50;
    test_alloc_different_free_blocks_in_range_8_to_24(region_size);
    region_size = 100;
    test_alloc_different_free_blocks_in_range_8_to_24(region_size);
    region_size = 200;
    test_alloc_different_free_blocks_in_range_8_to_24(region_size);
}

void test3() {
    int region_size;

    printf("test3\n");
    printf("-----------------------------------test different free block\n");
    printf("pri pouziti vacsich celkovych blokov pre spravcu pamate (aspon velkosti 1000 bytov)\n\n");
    region_size = 1000;
    test_alloc_different_free_blocks_in_range_500_to_5000(region_size);
    region_size = 5000;
    test_alloc_different_free_blocks_in_range_500_to_5000(region_size);
    region_size = 10000;
    test_alloc_different_free_blocks_in_range_500_to_5000(region_size);
}

void test4() {
    int region_size;

    printf("test4\n");
    printf("-----------------------------------test different free block\n");
    printf("pri pouziti vacsich celkovych blokov pre spravcu pamate (aspon velkosti 1000 bytov)\n\n");
    region_size = 1000;
    test_alloc_different_free_blocks_in_range_8_to_50000(region_size);
    region_size = 5000;
    test_alloc_different_free_blocks_in_range_8_to_50000(region_size);
    region_size = 10000;
    test_alloc_different_free_blocks_in_range_8_to_50000(region_size);
}

int main() {

    char region[50];

    memory_init(region, 50);
    char * end_add = (char *)((char *)memory_head + memory_head->end_off);

    printf("Size of MEM_HEAD: %d\n", sizeof(MEM_HEAD));
    printf("Size of BLOCK_HEAD: %d\n", sizeof(BLOCK_HEAD));
    printf("Size of ALLOC_HEAD: %d\n", sizeof(ALLOC_HEAD));
    printf("BEST FIT WITHOUT DYNAMIC HEADS\n\n");

    srand(time(0));

    // test1();
    // test2();
    // test3();
    // test4();

    test_connecting_free_block();

    return 0;
}