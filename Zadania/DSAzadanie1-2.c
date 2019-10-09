#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define SIZE_INIT 16                            //velkost inicializovanej pamate
#define MALLOC_SIZE 5                          //velkost pamate, ktoru chce pouzivatel alokovat
#define INT_OFFSET sizeof(int)                  //velkost int typu
#define CHAR_OFFSET sizeof(char)

/*struktura ("hlavicka") pre linked list blokov volnej pamate
    momentalna velkost: 4 */
typedef struct BLOCK_HEAD {
    int next_block_offset;
} BLOCK_HEAD;

static BLOCK_HEAD *memory_head = NULL;      //pointer na zaciatok volnych blokov
//ukazuje na hlavicku pamate, nie na volnu pouzitelnu pamat

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
int get_value_on(char *memory){
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
    BLOCK_HEAD *alloc_head = NULL;
    void *allocated_memory;                                           //ukazovatel na alokovanu pamat
    int next_block_head = get_value_on((char*)((char*)memory_head + memory_head->next_block_offset));
    int curr_mchunk_size;

    current = (BLOCK_HEAD*)((char*)memory_head + memory_head->next_block_offset);
    current->next_block_offset = next_block_head;
    curr_mchunk_size = current->next_block_offset - (int)INT_OFFSET;
    printf("%p %d\n",current, current->next_block_offset);

    if (current->next_block_offset != 1) {
        //first fit
        while(((curr_mchunk_size) < (size)) && (current->next_block_offset != 1)) { //najde prvy hodiaci sa volny blok
            previous = current;
            next_block_head = get_value_on((char*)((char*)current + current->next_block_offset));
            current = (BLOCK_HEAD*)((char*)current + current->next_block_offset);
            current->next_block_offset = next_block_head;
            curr_mchunk_size = current->next_block_offset - (int)INT_OFFSET;
        }
    } else {
        allocated_memory = NULL;
        printf("Error:\tnot enough memory\n");
        return allocated_memory;
    }

    alloc_head = current;
    allocated_memory = (char*)((char*)alloc_head + (int)INT_OFFSET);
    int alloc_size = (int) size;                                       //pretypovanie zadanej velkosti
    int remain_size = current->next_block_offset - (int)INT_OFFSET - alloc_size;//kolko zostane v bloku po alokacii
    printf("alloc_head ptr: %p\n", alloc_head);
    printf("allocated_memory ptr: %p\n", allocated_memory);
    printf("remain size: %d\n", remain_size);


    if (remain_size >= (int)INT_OFFSET) {
        current = (BLOCK_HEAD*)((char*)current + alloc_size + (int)INT_OFFSET);
        current->next_block_offset = remain_size;
        printf("%d %d\n", current->next_block_offset, get_value_on((char*)current));

        alloc_head->next_block_offset = (-1)*alloc_size;

        if (previous == NULL) {
            memory_head->next_block_offset = get_value_on((char*)current);
        } else {
            previous->next_block_offset = get_value_on((char*)current);
        }
        printf("After allocation, there remains extra space in chunk. (%d)\n", current->next_block_offset);
        return allocated_memory;

    } else if ((remain_size < (int)INT_OFFSET) && (remain_size >= 0)){
        next_block_head = get_value_on((char*)((char*)current + current->next_block_offset));
        current = (BLOCK_HEAD*)((char*)current + current->next_block_offset);

        alloc_head->next_block_offset = (-1)*curr_mchunk_size;

        memory_head->next_block_offset = (char*)current - (char*)memory_head;
        printf("%d %p\n", memory_head->next_block_offset, memory_head);

        if (next_block_head == 1) {
            printf("current is at the end.");
        } else {
            current->next_block_offset = next_block_head;
        }

        printf("After allocation, there not remains extra space in chunk.\n"
               "Allocated memory consumed whole block. (%d)\n", curr_mchunk_size);
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
    BLOCK_HEAD *free_block = (BLOCK_HEAD*)((char*)ptr + (int)INT_OFFSET);

    memory_head->next_block_offset = (char*)free_block - (char*)memory_head;
    free_block->next_block_offset = (char)(size - CHAR_OFFSET - INT_OFFSET);
    memset((char*)ptr + size - CHAR_OFFSET, 1, 1);

    printf("ptr: %p\n", ptr);
    printf("last ptr: %p\n", (char*)free_block + free_block->next_block_offset);
    printf("memory_h ptr: %p\n", memory_head);
    printf("free_block ptr: %p\n", free_block);
    printf("offset free_block to end: %d\n", free_block->next_block_offset);
    printf("Memory initialized-----------------------------\n\n");
}

//---------------------------------------------------------------------//
//                                                                     //
//       MAIN                                                          //
//                                                                     //
//---------------------------------------------------------------------//

int main() {
    char region[SIZE_INIT];
    char *ptr = (char*)memory_head;
    printf("%p\n", &region[SIZE_INIT-1]);
    memory_init(region, SIZE_INIT);

    for (int i = 0; i < SIZE_INIT; ++i) {
        printf("%d %p\n", region[i], &region[i]);
    }

    char * pointer = (char*) memory_alloc(MALLOC_SIZE);
    printf("%p\n", pointer);
    if (pointer)
        memset(pointer, 0, MALLOC_SIZE);

    for (int i = 0; i < SIZE_INIT; ++i) {
        printf("%d %p\n", region[i], &region[i]);
    }

//    char * pointer1 = (char*) memory_alloc(2);
//    if (pointer1)
//        memset(pointer1, 0, 2);

//    for (int i = 0; i < SIZE_INIT; i++) {
//        printf("%d %p\n", region[i], &region[i]);
//    }

//    if (pointer)
//        memory_free(pointer);
    return 0;
}