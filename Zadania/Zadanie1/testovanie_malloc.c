#include <stdlib.h>
#include <time.h>
#include "zadanie1_L.c"
#include "zadanie1_S.c"

//testovanie fungovania spajania pri free

int get_random_range(int low, int up) 
{  
    return (rand() % (up - low + 1)) + low;
} 

//---------------------------------------------------------------------//
//                                                                     //
//       Test same free blocks                                         //
//                                                                     //
//---------------------------------------------------------------------//

void test1_same_free_blocks(int region_size) {
    char region[region_size]; //blok pamate, na ktorom bude realizovana alokacia
    memory_init(region, region_size); //inicializacia volneho bloku pamate

    int alloc_block_size = get_random_range(8, 24);
    BLOCK_HEAD * first_free_block = (BLOCK_HEAD *)((char *)memory_head + memory_head->first_free_offset);

    void * array_p[region_size];

    for (int i = 0; i < region_size; i++)
    {
        array_p[i] = memory_alloc(alloc_block_size);

        if (array_p[i] == NULL) {
            printf("-----TEST ENDED-----\n");
            int remain_size = get_int_value_on((char *)memory_head + memory_head->first_free_offset);
            int taken = (i * (alloc_block_size + sizeof(ALLOC_HEAD))) + sizeof(MEM_HEAD) + CHAR_OFFSET + sizeof(BLOCK_HEAD);
            float efficency = (float)remain_size / first_free_block_size * 100.0;
            if (remain_size > alloc_block_size || remain_size < 0) {
                remain_size = 0;
                efficency = 100.00;
                taken -= sizeof(BLOCK_HEAD);
            }
            printf("Itterations: %d\n", i);
            printf("Alloc block size: %d\n", alloc_block_size);
            printf("Taken memory: %d/%d\n", taken, region_size);
            printf("Unalloced memory size: %d\n", remain_size);
            printf("Efficency: %.02f%%\n", efficency);
            break;
        }
    }
}

//rovnake bloky malej velkosti (8 az 24 bytov)
//maly blok pamate (do 50 bytov, do 100 bytov, do 200 bytov)
void test_memory_same_free_blocks() {
    int region_size = 50;
    test_same_free_blocks(region_size);

    region_size = 100;
    test_same_free_blocks(region_size);

    region_size = 200;
    test_same_free_blocks(region_size);
}

//---------------------------------------------------------------------//
//                                                                     //
//       Test differen free blocks                                     //
//                                                                     //
//---------------------------------------------------------------------//

void test_different_free_blocks(int region_size) {
    char region[region_size]; //blok pamate, na ktorom bude realizovana alokacia
    memory_init(region, region_size); //inicializacia volneho bloku pamate
}

//nerovnake blokoy malej velkosti (nahodne 8 až 24 bytov) 
//malých blok pamate (do 50 bytov, do 100 bytov, do 200 bytov)
void test_memory_different_free_blocks() {
    int region_size = 50;
    test_same_free_blocks(region_size);

    region_size = 100;
    test_same_free_blocks(region_size);

    region_size = 200;
    test_same_free_blocks(region_size);
    

}


//  prideľovanie nerovnakých blokov väčšej veľkosti (veľkosti 500 až 5000 bytov) pri použití väčších celkových blokov pre správcu pamäte (aspoň veľkosti 1000 bytov),
//  prideľovanie nerovnakých blokov malých a veľkých veľkostí (veľkosti od 8 bytov do 50 000) pri použití väčších celkových blokov pre správcu pamäte (aspoň veľkosti 1000 bytov).

//---------------------------------------------------------------------//
//                                                                     //
//       Test free function                                            //
//                                                                     //
//---------------------------------------------------------------------//


//---------------------------------------------------------------------//
//                                                                     //
//       MAIN                                                          //
//                                                                     //
//---------------------------------------------------------------------//

int main() {

    // printf("Size of MEM_HEAD: %d\n", sizeof(MEM_HEAD));
    // printf("Size of BLOCK_HEAD: %d\n", sizeof(BLOCK_HEAD));
    // printf("Size of ALLOC_HEAD: %d\n\n", sizeof(ALLOC_HEAD));
    // srand(time(0));
    // test_memory_same_free_blocks();
    return 0;
}