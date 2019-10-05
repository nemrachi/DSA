#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define SIZE_INIT 50                            //velkost inicializovanej pamate
#define MALLOC_SIZE 37                          //velkost pamate, ktoru chce pouzivatel alokovat
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
    while((current->mchunk_size < (size + INT_OFFSET)) && (current->next != NULL)) { //najde prvy hodiaci sa volny blok
        previous = current;
        current = current->next;
    }

    if(current->mchunk_size + INT_OFFSET == (size + INT_OFFSET)) {                 //ak blok je rovnako velky ako velkost alokovanej
                                                                      // pamate+hlavicka
        if (previous != NULL) {                                       //ak nie je blok prvy

        }
        previous = current;
        current =
        current->mchunk_size = ~current->mchunk_size + 1;
        printf("original ptr alloc:%p\n", current);
        allocated_memory = (void*) ++current;    //jump to address for user data (address after header)
        printf("++ptr alloc:%p\n", allocated_memory);
        printf("Allocated memory is same size as initialized memory.");
        return allocated_memory;

    } else if (current->mchunk_size > (size + HEAD_SIZE)) {
        if (current->mchunk_size - size < HEAD_SIZE) { //ak by zostal fragment mensi ako samotna hlavicka free chunku, daj mallocu vsetku pamet
            allocated_memory = current;
            memset(allocated_memory, (int)size, INT_OFFSET); //ulozila som velkost allocovanej pamate do jeho hlavicky
            allocated_memory = allocated_memory + INT_OFFSET; //posunula som sa na realne vyuzitie pamate
            if (previous == NULL) {
                h_free_mchunks = current;
            } else {
                previous->next = current->next;
            }
        } else {
            allocated_memory = current;
            memset(allocated_memory, (int)size, INT_OFFSET); //ulozila som velkost allocovanej pamate do jeho hlavicky
            allocated_memory = allocated_memory + INT_OFFSET; //posunula som sa na realne vyuzitie pamate
            current = allocated_memory + size;
            if (previous == NULL) {
                h_free_mchunks = current;
            } else {
                previous->next = current;
            }
        }

        printf("Allocated block fit in init memory with a split\n");
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
//    if (pointer)
//        memory_free(pointer);
    return 0;
}
