#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define SIZE_INIT 50                            //velkost inicializovanej pamate
#define MALLOC_SIZE 39                          //velkost pamate, ktoru chce pouzivatel alokovat
#define INT_OFFSET sizeof(int)                  //velkost int typu

/*struktura ("hlavicka") pre linked list blokov volnej pamate
    momentalna velkost: 8 */
typedef struct FREE_MCHUNK {
    int mchunk_size;                            //velkost pamate, ktoru moze pouzivatel REALNE vyuzit
                                                //dalej v programe->mchunk_size+INT_OFFSET, lebo je rozdiel medzi hlavickami
    struct FREE_MCHUNK *next;                   //odkaz na dalsi volny blok
} FREE_MCHUNK;

#define HEAD_SIZE sizeof(FREE_MCHUNK)          //velkost hlavicky volnych blokov

static FREE_MCHUNK *h_free_mchunks = NULL;      //pointer na zaciatok volnych blokov
                                                //ukazuje na hlavicku, nie na pouzitelnu pamat

//---------------------------------------------------------------------//
//                                                                     //
//       SECONDARY FUNCTIONS                                           //
//                                                                     //
//---------------------------------------------------------------------//

//funkcia vrati ukazovatel typu int na hlavicku
int *getOnMAddress(char *memory){
    return (int*)memory;
}

//funkcia vráti hodnotu napísanu v hlavičke
int getValOn(char *memory){
    return *getOnMAddress(memory);
}

//---------------------------------------------------------------------//
//                                                                     //
//       MAIN 4 FUNCTIONS                                              //
//                                                                     //
//---------------------------------------------------------------------//

/*funckia na alokaciu pamate
atributy: velkost pamate, ktoru chce pouzivatel vyuzivat*/
void *memory_alloc(unsigned int size) {
    printf("Memory of %dB starting allocating--------------\n", size);

    FREE_MCHUNK *previous = NULL, *current = NULL;                    //docasne ukazovatele na volnu pamat
    void *allocated_memory;                                           //ukazovatel na alokovanu pamat
    current = h_free_mchunks;

    //first fit
    while((current->mchunk_size < (size)) && (current->next != NULL)) { //najde prvy hodiaci sa volny blok
        previous = current;
        current = current->next;
    }

    allocated_memory = current;
    int alloc_size = (int) size;                                       //pretypovanie zadanej velkosti
    int remain_mchunk = current->mchunk_size - alloc_size;//kolko zostane v bloku po alokacii
    printf("current ptr: %p\nprev ptr: %p\n", current, previous);
    printf("current next: %p\n", current->next);


    if (remain_mchunk >= HEAD_SIZE) {
        current = current + alloc_size + HEAD_SIZE;
        current->mchunk_size = remain_mchunk;
        memset(current, current->mchunk_size, 1);
        //memset(current+INT_OFFSET, (int) current->next, INT_OFFSET);
        //neviem, ci prepisem pointer na next node

        memset(allocated_memory, alloc_size, 1);
        allocated_memory += INT_OFFSET;
        memset(allocated_memory, 1, 1);
        allocated_memory += INT_OFFSET;

        if (previous == NULL) {
            h_free_mchunks =  current;
        } else {
            previous->next = current;
        }
        printf("After allocation, there remains extra space in chunk.");
        return allocated_memory;

    } else if ((remain_mchunk < HEAD_SIZE) && (remain_mchunk >= 0)){
        int curr_size = current->mchunk_size;
        current = current->next;
        if (current == NULL) {
            h_free_mchunks = NULL;
        } else {
            memset(current, current->mchunk_size, 1);
            memset(current+INT_OFFSET, (int) current->next, 1);
        }

        memset(allocated_memory, curr_size, 1);
        allocated_memory += INT_OFFSET;
        memset(allocated_memory, 1, 1);
        allocated_memory += INT_OFFSET;
        printf("allocate memory head:\n %d\n %d\n", getValOn(allocated_memory-HEAD_SIZE), getValOn(allocated_memory-INT_OFFSET));
        printf("After allocation, there not remains extra space in chunk.\nAllocated memory consumed whole block.\n");
        return allocated_memory;

    } else {
        allocated_memory = NULL;
        printf("Error:\tnot enough memory\n");
        return allocated_memory;
    }
}

//memory free function
int memory_free(void *valid_ptr) {

}

//check
int memory_check(void *ptr){

}

void memory_init(void *ptr, unsigned int size) {
    printf("Memory starting initializing-------------------\n");

    h_free_mchunks = ptr;                       //ukazovatel na volny blok pamate
    size = size - HEAD_SIZE;                    //realna velkost pamate, ktora sa moze vyuzit
    h_free_mchunks->mchunk_size = (int)size;    //ulozenie aktualnej velkosti do hlavicky
    h_free_mchunks->next = NULL;

    printf("address of init memory: %p\n", h_free_mchunks);
    printf("size    of init memory: %d\n", h_free_mchunks->mchunk_size);
    printf("Memory initialized-----------------------------\n");
}

//---------------------------------------------------------------------//
//                                                                     //
//       MAIN                                                          //
//                                                                     //
//---------------------------------------------------------------------//

int main() {
    char region[SIZE_INIT];
    memory_init(region, SIZE_INIT);
    char * pointer = (char*) memory_alloc(MALLOC_SIZE);
    if (pointer)
        memset(pointer, 0, MALLOC_SIZE);
    char * pointer1 = (char*) memory_alloc(2);
    if (pointer1)
        memset(pointer, 0, 2);


//    if (pointer)
//        memory_free(pointer);
    return 0;
}
