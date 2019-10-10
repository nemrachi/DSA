#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define SIZE_INIT 50                            //velkost inicializovanej pamate
#define MALLOC_SIZE 20                          //velkost pamate, ktoru chce pouzivatel alokovat
#define INT_OFFSET sizeof(int)                  //velkost int typu

/*struktura ("hlavicka") pre linked list blokov volnej pamate
    momentalna velkost: 8 */
//typedef struct BLOCK_HEAD {
//    int mchunk_size;                            //velkost pamate, ktoru moze pouzivatel REALNE vyuzit
//    //dalej v programe->mchunk_size+INT_OFFSET, lebo je rozdiel medzi hlavickami
//    struct BLOCK_HEAD *next;                   //odkaz na dalsi volny blok
//} BLOCK_HEAD;

#define HEAD_SIZE sizeof(BLOCK_HEAD)          //velkost hlavicky volnych blokov

static BLOCK_HEAD *memory_head = NULL;      //pointer na zaciatok volnych blokov
//ukazuje na hlavicku, nie na pouzitelnu pamat

//---------------------------------------------------------------------//
//                                                                     //
//       SECONDARY FUNCTIONS                                           //
//                                                                     //
//---------------------------------------------------------------------//

//funkcia vrati ukazovatel typu int na hlavicku
int *get_on_memory_address(char *memory){
    return (int*)memory;
}

//funkcia vráti hodnotu napísanu v hlavičke
int get_int_value_on(char *memory){
    return *get_on_memory_address(memory);
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

    BLOCK_HEAD *previous = NULL, *current = NULL;                    //docasne ukazovatele na volnu pamat
    BLOCK_HEAD *curr_next = NULL, *alloc_head = NULL;
    void *allocated_memory;                                           //ukazovatel na alokovanu pamat
    current = memory_head->next;

    if (current != NULL) {
        //first fit
        while((current->mchunk_size < (size)) && (current->next != NULL)) { //najde prvy hodiaci sa volny blok
            previous = current;
            current = current->next;
        }
    } else {
        allocated_memory = NULL;
        printf("Error:\tnot enough memory\n");
        return allocated_memory;
    }

    curr_next = current->next;
    alloc_head = current;
    allocated_memory = (char*)((char*)alloc_head + (int)INT_OFFSET);
    int alloc_size = (int) size;                                       //pretypovanie zadanej velkosti
    int remain_mchunk = current->mchunk_size - alloc_size;//kolko zostane v bloku po alokacii
    printf("alloc_head ptr: %p\n", alloc_head);
    printf("allocated_memory ptr: %p\n", allocated_memory);


    if (remain_mchunk >= (int)HEAD_SIZE) {
        current = (BLOCK_HEAD*)((char*)current + alloc_size + (int)INT_OFFSET);
        current->mchunk_size = remain_mchunk - (int)INT_OFFSET;
        current->next = curr_next;
        printf("%d %d\n", current->mchunk_size, get_int_value_on((char *) current));

        alloc_head->mchunk_size = (-1)*alloc_size;
        //alloc_head->next = NULL;  !!!!!!!!prepisuje mi to hodnotu current->mchunk_size

        if (previous == NULL) {
            memory_head->next = current;
        } else {
            previous->next = current;
        }
        printf("After allocation, there remains extra space in chunk. (%d)\n", current->mchunk_size);
        return allocated_memory;

    } else if ((remain_mchunk < (int)HEAD_SIZE) && (remain_mchunk >= 0)){
        alloc_head->mchunk_size = (-1)*current->mchunk_size;
        alloc_head->next = NULL;

        current = current->next;
        if (current == NULL) {
            memory_head->next = NULL;
        } else {
            printf("current moved on next node... same attributes of note remains");
        }

        printf("After allocation, there not remains extra space in chunk.\nAllocated memory consumed whole block.\n");
        printf("current->next: %p", current);
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
    memory_head = (BLOCK_HEAD*)ptr;                       //ukazovatel na volny blok pamate
    BLOCK_HEAD *free_block = (BLOCK_HEAD*)((char*)ptr + (int)HEAD_SIZE); //ok

    memory_head->mchunk_size = (int)size;    //velkost init pamate

    size = size - 3*(int)INT_OFFSET;           //realna velkost pamate, ktora sa moze vyuzit
    free_block->mchunk_size = (int)size;
    free_block->next = NULL;

    memory_head->next = free_block;

    printf("ptr: %p\n", ptr);
    printf("memory_h ptr: %p\n", memory_head);
    printf("free_block ptr: %p\n", free_block);
    printf("size of free_block: %d\n", get_int_value_on((char *) free_block));
    printf("Memory initialized-----------------------------\n\n");
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

//    for (int i = 0; i < SIZE_INIT; i++) {
//        printf("%d %p\n", region[i], &region[i]);
//    }

    char * pointer1 = (char*) memory_alloc(2);
    if (pointer1)
        memset(pointer1, 0, 2);

//    for (int i = 0; i < SIZE_INIT; i++) {
//        printf("%d %p\n", region[i], &region[i]);
//    }

//    if (pointer)
//        memory_free(pointer);
    return 0;
}